﻿using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;

namespace LN
{
	

    /// <summary>
    /// ライブラリ内部で発生した例外
    /// </summary>
    public class LNoteException : Exception
    {
        /// <summary>
        /// エラーコード
        /// </summary>
        public ErrorCode ErrorCode { get; private set; }

        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name=""errorCode"">エラーコード</param>
        public LNoteException(ErrorCode errorCode)
        {
            ErrorCode = errorCode;
        }
    }

    internal enum _LNInternal
    {
        InternalBlock,
    }   

    /// <summary>
    /// 参照カウントを持つオブジェクトをラップする
    /// </summary>
    public class ReferenceObject : IDisposable
    {
        internal static bool SystemInitialized = false;
        internal IntPtr _handle;

        internal ReferenceObject() {}
        internal ReferenceObject(_LNInternal i) {}

        /// <summary>
        /// ハンドルの取得
        /// </summary>
        public IntPtr Handle { get { return _handle; } }

        /// <summary>
        /// デストラクタ
        /// </summary>
        ~ReferenceObject() { Dispose(); }

        internal virtual void SetHandle(IntPtr handle)
        {
            _handle = handle;
        }       

        /// <summary>
        /// Dispose
        /// </summary>
        public void Dispose()
        {
            if (_handle != IntPtr.Zero)
            {
                if (SystemInitialized)
                    API.LNObject_Release(_handle);
                _handle = IntPtr.Zero;
            }
            GC.SuppressFinalize(this);
        }
    }

    internal class TypeInfo
    {
        public delegate ReferenceObject ReferenceObjectFactory(IntPtr handle);

        public ReferenceObjectFactory Factory;

        private static List<TypeInfo> _typeInfos = new List<TypeInfo>();

        public static void Register()
        {

        }

        public static TypeInfo GetTypeInfoByHandle(IntPtr handle)
        {
            int index = (int)LNObject_GetTypeUserData(handle);
            return _typeInfos[index];
        }

        [DllImport(API.DLLName, CallingConvention = API.DefaultCallingConvention)]
        private static extern IntPtr LNObject_GetTypeUserData(IntPtr handle);

    }

    /// <summary>
    /// オブジェクトのコレクション
    /// </summary>
    public class ObjectList<T> : ReferenceObject
        where T : ReferenceObject
    {
        private List<ReferenceObject> _list;
        
        internal ObjectList(_LNInternal i) {}

        internal override void SetHandle(IntPtr handle)
        {
            _list = new List<ReferenceObject>();
            _handle = handle;
            int count = Count;
            for (int i = 0; i < count; i++)
            {
                IntPtr item;
                API.LNObjectList_GetAt(_handle, i, out item);
                var t = TypeInfo.GetTypeInfoByHandle(item).Factory(item);
                t.SetHandle(item);
                _list.Add(t);
            }
        }
        
        public int Count
        {
            get
            {
                int count;
                API.LNObjectList_GetCount(_handle, out count);
                return count;
            }
        }
        
        public T this[int index]
        {
            get
            {
                SyncItems();
                return (T)_list[index];
            }
            set
            {
                SyncItems();
                _list[index] = value;
                API.LNObjectList_SetAt(_handle, index, value.Handle);
            }
        }
        
        public void Add(T item)
        {
            SyncItems();
            _list.Add(item);
            API.LNObjectList_Add(_handle, item.Handle);
        }
        
        public void Clear()
        {
            _list.Clear();
            API.LNObjectList_Clear(_handle);
        }
        
        public void Insert(int index, T item)
        {
            SyncItems();
            _list.Insert(index, item);
            API.LNObjectList_Insert(_handle, index, item.Handle);
        }
        
        public void Remove(T item)
        {
            SyncItems();
            _list.Remove(item);
            API.LNObjectList_Remove(_handle, item.Handle);
        }
        
        public void RemoveAll(T item)
        {
            SyncItems();
            _list.RemoveAll((i) => i == item);
            API.LNObjectList_RemoveAll(_handle, item.Handle);
        }
        
        public void RemoveAt(int index)
        {
            SyncItems();
            _list.RemoveAt(index);
            API.LNObjectList_RemoveAt(_handle, index);
        }

        private void SyncItems() 
        {
            int count = Count;
            if (_list.Count < count)
            {
                // 足りない分を詰める
                for (int i = 0; i < count - _list.Count; ++i)
		        {
                    _list.Add(null);
                }

                // リスト内容を同期する
                for (int i = 0; i < count; ++i)
		        {
                    IntPtr item;
                    API.LNObjectList_GetAt(_handle, i, out item);
			        if (_list[i] == null || _list[i].Handle != item)
			        {
                        var t = TypeInfo.GetTypeInfoByHandle(item).Factory(item);
                        t.SetHandle(item);
                        _list.Add(t);
			        }
		        }
            }
        }
    }

}
