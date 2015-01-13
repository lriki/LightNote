# coding: utf-8
#==============================================================================
# ■ CSXMLMain
#------------------------------------------------------------------------------
#   このファイルは次のファイルから実行される。
#   Source/DotNet/LightNote.DotNet/LightNote.DotNet/ConvertXMLDocSource.bat
#
#   「//#CLASS_BODY.モジュール名」のある個所に、
#   解析して生成されたメソッド定義が展開される。
#==============================================================================
require_relative './Analyzer/FuncLibAnalyzer.rb'
require_relative './Analyzer/CSUtils.rb'

$func_xmldoc_summary_template = "        /// <summary>\n        /// SUMMARY\n        /// </summary>"
$func_xmldoc_param_template = "        /// <param name=\"PARAM_NAME\">DETAIL</param>"
$func_xmldoc_return_template = "        /// <returns>DETAIL</returns>"

$func_dispose_template = "public void Dispose()\n{\n    if (_handle != IntPtr.Zero) {\n        API.API_NAME(_handle);\n        _handle = IntPtr.Zero;\n    }\n    GC.SuppressFinalize(this);\n}\n"

$handle_wrapper_base_class = "ReferenceObjectWrapper"

#==============================================================================
# ▲ 設定ここまで
#==============================================================================

class PropertyInfo
  attr_accessor :type          # C++ 型名
  attr_accessor :get_func_info # get 定義情報 (FuncInfo)
  attr_accessor :set_func_info # set 定義情報 (FuncInfo)
  def initialize
    @type = nil
    @get_func_info = nil
    @set_func_info = nil
  end
end

#==============================================================================
#
#==============================================================================

$this_field_name = "_handle";
INSTANCE_HANDLE_TYPE = "lnHandle"

class PropertyAnalyzer
  #----------------------------------------------------------------------------
  # 
  #----------------------------------------------------------------------------
  attr_accessor :class_prop_hash# プロパティリスト {プロパティ名, PropertyInfo}
  #----------------------------------------------------------------------------
  # 
  #----------------------------------------------------------------------------
  def initialize
    @class_prop_hash = {}
  end
  #----------------------------------------------------------------------------
  # 
  #----------------------------------------------------------------------------
  def clear
    @class_prop_hash.clear
  end
  #----------------------------------------------------------------------------
  # 
  #----------------------------------------------------------------------------
  def try_push(func_info)
    unless func_info.func_opt.cs.property.empty?
      # エラーチェック。プロパティにできるのは、第一引数が this を示す handle で、全引数数が2個以下のモノに限る
      raise "invalid property - this handle.\n" + func_info.func_name if func_info.func_decl.arg_list[0].type != "lnHandle"
      raise "invalid property - args count.\n" + func_info.func_name if func_info.func_decl.arg_list.size > 2
    
      # 新しく作成
      unless @class_prop_hash.has_key?(func_info.func_opt.cs.property[0])
        # set は先に登録できない (value.type への格納方法を修正する必要がある)
        #raise "can not be registered before the set. ..." + func_info.func_name + "\n" if func_info.func_opt.cs.property[1] == "set"
      
        info = PropertyInfo.new
        
        type = func_info.func_decl.return_type
        if type == "void" # 戻り値が void であればふたつめの引数の型を使う
          type = func_info.func_decl.arg_list[1].type
        end
        
        info.type = type # ここではまだ C++ の型のまま   #func_info.func_opt.cs.get_host_field_type(type)
        @class_prop_hash[func_info.func_opt.cs.property[0]] = info
      end
      
      # get set 振り分け
      info = @class_prop_hash[func_info.func_opt.cs.property[0]]
      
      if func_info.func_opt.cs.property[1] == "get"
        info.get_func_info = func_info
      elsif func_info.func_opt.cs.property[1] == "set"
        info.set_func_info = func_info
      end
      
      # この定義はスキップ
      return true
    end
    return false
  end  
  #----------------------------------------------------------------------------
  # 
  #----------------------------------------------------------------------------
  def dump
    output_text = ""
  
    @class_prop_hash.each{|key, value|
    
      host_field_type = nil
      host_field_type = value.get_func_info.func_opt.cs.host_field_type if value.get_func_info != nil and value.get_func_info.func_opt.cs.host_field_type != nil
      host_field_type = value.set_func_info.func_opt.cs.host_field_type if value.set_func_info != nil and value.set_func_info.func_opt.cs.host_field_type != nil
      
      # 戻り値が明示されている場合はそれを使用する
      if host_field_type != nil
        output_text += "public " + host_field_type + " " + key + " {\n"
      else
        output_text += "public " + convert_type_to_cs_no_modifier(value.type) + " " + key + " {\n"
      end
      
      cs_no_modifier_type = convert_type_to_cs_no_modifier(value.type)
      
      #-------------------------------------------
      # getter
      if value.get_func_info != nil
        output_text += "    get {\n"
        
        # 値型 (プリミティブ型) の場合
        if value_cs_primitive_type?(cs_no_modifier_type)
          text  = "return API.API_NAME(THIS_FIELD_NAME);\n"
          text.gsub!("API_NAME", value.get_func_info.func_name)
          text.gsub!("THIS_FIELD_NAME", $this_field_name)
          
        # 値型 (struct) の場合
        elsif cs_struct_type?(cs_no_modifier_type)
          text  = "VALUE_TYPE v = new VALUE_TYPE();\n"
          text += "API.API_NAME(THIS_FIELD_NAME, ref v);\n"
          text += "return v;\n"
          text.gsub!("VALUE_TYPE", convert_type_to_cs_no_modifier(value.type))
          text.gsub!("API_NAME", value.get_func_info.func_name)
          text.gsub!("THIS_FIELD_NAME", $this_field_name)
        
        # 参照型の場合
        else
          opt = value.get_func_info.func_opt.cs
          raise "none opt.host_field. [" + value.get_func_info.func_name + "]\n" if opt.host_field == nil
          
          # get テンプレート
          text  = "if (HOLD_FIELD_NAME == null) {\n"
          text += "    HOLD_FIELD_NAME = new WRAPPER_CLASS(API.API_NAME(THIS_FIELD_NAME));\n"
          text += "}\n"
          text += "return HOLD_FIELD_NAME;\n"
          text.gsub!("HOLD_FIELD_NAME", opt.host_field)
          text.gsub!("WRAPPER_CLASS", opt.host_field_type)
          text.gsub!("API_NAME", value.get_func_info.func_name)
          text.gsub!("THIS_FIELD_NAME", $this_field_name)
        end
      
        output_text += text + "    }\n"
      end
      
      #-------------------------------------------
      # setter
      if value.set_func_info != nil
        output_text += "set {\n"
        
        # 値型 (プリミティブ型) の場合
        if value_cs_primitive_type?(cs_no_modifier_type)
          text  = "API.API_NAME(THIS_FIELD_NAME, value);\n"
          text.gsub!("API_NAME", value.set_func_info.func_name)
          text.gsub!("THIS_FIELD_NAME", $this_field_name)
          
        # 値型の場合
        elsif cs_struct_type?(cs_no_modifier_type)
          # set テンプレート
          text = "API.API_NAME(THIS_FIELD_NAME, ref value);\n"
          text.gsub!("API_NAME", value.set_func_info.func_name)
          text.gsub!("THIS_FIELD_NAME", $this_field_name)
          
        # 参照型の場合
        else
          # set テンプレート
          text = "API.API_NAME(THIS_FIELD_NAME, value.THIS_FIELD_NAME);\n"
          text.gsub!("API_NAME", value.set_func_info.func_name)
          text.gsub!("THIS_FIELD_NAME", $this_field_name)
        end
      
        output_text += text + "}\n"
      end
      
      output_text += "}\n"
    }
    
    return output_text
  end
end



#==============================================================================
#
#==============================================================================

# C++ ヘッダ解析
analyzer = FuncLibAnalyzer.new
analyzer.analyze($target_files)

# enum 解析
$enum_analyzer = EnumAnalyzer.new
for file_path in $enum_files
  $enum_analyzer.analyze($lnote_root + file_path)
end

# プロパティ解析
$property_analyzer = PropertyAnalyzer.new

# C# ソースを 1 行ずつ読んでいく
$output = ""
$class_body_output = ""
file = File.open(ARGV[0], "r:utf-8")
while line_text = file.gets do
  if line_text.include?("//#")
    # 空白削除 > "//" #削除 > "." で分割
    idents = line_text.strip.delete("//#").split(".")
    
    handle_wrapper_class = idents.include?($handle_wrapper_base_class)
    
    # CLASS_BODY であればメソッド定義を展開する
    if idents[0] == "CLASS_BODY"
      $class_body_output = ""
      $property_analyzer.clear
      
      # オブジェクトハンドル (とりあえずここに置いておく)
      #$class_body_output = "private IntPtr _handle;\n\n"
      
      # フィールドリスト {フィールド名, C++型名}
      class_field_hash = {}
      
      
      # 指定モジュール名の関数一覧
      func_list = analyzer.find_func_info_list_by_module(idents[1])
      for func_info in func_list
      
        next unless func_info.func_opt.cs.enable
        
         # フィールドに保持する値を返す関数の場合
        if func_info.func_opt.cs.host_field != nil
          # 追加するフィールドとする
          class_field_hash[func_info.func_opt.cs.host_field] = func_info.func_opt.cs.get_host_field_type(func_info.func_decl.return_type)
        end
        
        #------------------------------------------------------
        # プロパティ解析 (登録できたらこの関数はスキップ)
        next if $property_analyzer.try_push(func_info)
        
        #------------------------------------------------------
        # メソッド名
        # コンストラクタである
        if func_info.func_opt.cs.constructor
          # アクセスレベル
          $class_body_output += "public "
          
          # モジュール名をクラス名に
          $class_body_output += convert_module_to_class_name(func_info.func_decl.module_name)
          
        # デストラクタである
        elsif func_info.func_opt.cs.destructor
          $class_body_output += "~" + convert_module_to_class_name(func_info.func_decl.module_name)
          
        # 普通のメソッドである
        else
          # アクセスレベル
          $class_body_output += "public "
          
          # static 有無
          $class_body_output += "static " if func_info.func_opt.cs.method_kind == STATIC_METHOD
          
          # 戻り値
          if func_info.func_opt.cs.return_out_arg > -1
            type = func_info.func_decl.arg_list[func_info.func_opt.cs.return_out_arg].type
            $class_body_output += convert_type_to_cs_basic(type) + " "
          else
            $class_body_output += convert_type_to_cs(func_info.func_decl.return_type) + " " # Type C++ > C#
          end
          
          # 関数名
          if func_info.func_opt.cs.method_name != nil
            $class_body_output += func_info.func_opt.cs.method_name # メソッド名を別名に
          else
            $class_body_output += func_info.func_decl.method_name
          end
        end
        
        #------------------------------------------------------
        # 仮引数リスト
        $class_body_output += "("
        args_text = ""
        
        # デストラクタ以外
        unless func_info.func_opt.cs.destructor
          for i in 0...func_info.func_decl.arg_list.size
            arg = func_info.func_decl.arg_list[i]
            # インスタンスメソッドの場合は先頭の IntPtr をスキップ
            next if i == 0 and func_info.func_opt.cs.method_kind == CLASS_INSTANCE and arg.type == INSTANCE_HANDLE_TYPE
            # struct メソッドの場合はすぐスキップ
            next if i == 0 and func_info.func_opt.cs.method_kind == STRUCT_INSTANCE
            # 戻り値として返したい引数の場合はスキップ (Vector3.Dot とか)
            next if i == func_info.func_opt.cs.return_out_arg
            # テキストを作る
            args_text += ", " if args_text != ""  # 2つ目以降の引数なら , を追加
            args_text += convert_type_to_cs_parameter(arg.type) + " " # Type C++ > C#
            args_text += arg.name
            args_text += "= " + arg.default if arg.default != nil
          end
        end
        $class_body_output += args_text + ")\n"
        
        #------------------------------------------------------
        # コンストラクタの場合は base コール
        if func_info.func_opt.cs.constructor
          $class_body_output += "    : base(IntPtr.Zero)\n"
        end
      
        #------------------------------------------------------
        # デストラクタ用特殊処理
        if func_info.func_opt.cs.destructor
          $class_body_output += "{ Dispose(); }\n"
          $class_body_output += $func_dispose_template.gsub("API_NAME", func_info.func_decl.func_name)
        end
        
        #------------------------------------------------------
        # 関数本体
        
        # デストラクタ以外
        unless func_info.func_opt.cs.destructor
          # 関数本体開始
          $class_body_output += "{\n"
          #$class_body_output += " " * 4
          
          
          # 一時変数に格納してそれを返したい
          if func_info.func_opt.cs.return_out_arg > -1
            type     = func_info.func_decl.arg_list[func_info.func_opt.cs.return_out_arg].type
            arg_name = func_info.func_decl.arg_list[func_info.func_opt.cs.return_out_arg].name
            $class_body_output += "var " + arg_name + " = new " + convert_type_to_cs_basic(type) + "();\n"
          end
          
          # 戻り値の型はあるか
          unless func_info.func_decl.return_type.empty?
            # フィールドに戻り値を格納する場合
            if func_info.func_opt.cs.host_field != nil
              $class_body_output += func_info.func_opt.cs.host_field + " = "
            elsif func_info.func_decl.return_type != "void"
              $class_body_output += "return "
            end
          end
          
          # メソッドコール
          args_text = ""
          $class_body_output += "API." + func_info.func_decl.func_name + "(";
          
          # 実引数リスト
          #for arg in func_info.func_decl.arg_list
          for i in 0...func_info.func_decl.arg_list.size
            arg = func_info.func_decl.arg_list[i]
            # インスタンスメソッドの場合は先頭の IntPtr を _handle に
            if i == 0 and func_info.func_opt.cs.method_kind == CLASS_INSTANCE and arg.type == INSTANCE_HANDLE_TYPE
              args_text += $this_field_name
              next
            end
            # struct メソッドの場合は this を渡す
            if i == 0 and  func_info.func_opt.cs.method_kind == STRUCT_INSTANCE
              args_text += "ref this"
              next
            end
            # 2つ目以降の引数なら , を追加
            args_text += ", " if args_text != ""  
            args_text += convert_arg_name_to_cs_for_api(arg)
          end
          $class_body_output += args_text + ");\n";
          
          # 一時変数に格納してそれを返したい
          if func_info.func_opt.cs.return_out_arg > -1
            $class_body_output += "return " + func_info.func_decl.arg_list[func_info.func_opt.cs.return_out_arg].name + ";\n"
          end

          # 関数本体終了
          $class_body_output += "}\n\n"
          
        end
      end
      
      #------------------------------------------------------
      # プロパティ定義
      $class_body_output += $property_analyzer.dump
      
      #------------------------------------------------------
      # フィールド定義
      $class_body_output += "\n"
      class_field_hash.each{|key, value|
        if key != "_handle"   # _handle は定義しない
          $class_body_output += "internal " + convert_type_to_cs(value) + " " + key + ";\n"
        end
      }
      
      #------------------------------------------------------
      # 必須メソッド
      
      # 空コンストラクタ
=begin
      text  = "        internal CLASS_NAME()\n"
      text += "        {\n"
      text += "        }\n"
      text.gsub!("CLASS_NAME", convert_module_to_class_name(idents[1]))
      $class_body_output += text
=end

      # ハンドルコンストラクタ
      if handle_wrapper_class
        text  = "        internal CLASS_NAME(IntPtr handle)\n"
        text += "            : base(handle)\n"
        text += "        {\n"
        text += "        }\n"
        text.gsub!("CLASS_NAME", convert_module_to_class_name(idents[1]))
        $class_body_output += text
      end
    
      # 出力して次の行へ
      $output += $class_body_output
      next
      
    # それ以外はXMLドキュメントを置換する
    else
      # 関数検索
      func_info = analyzer.find_func_info(idents[0])
      if func_info == nil
        p "not found func ... " + idents[0]
      else
        # <summary>
        if idents[1] == "summary"
          $output += $func_xmldoc_summary_template.gsub("SUMMARY",func_info.func_doc.summary) + "\n";
          next
        # <param>
        elsif idents[1] == "param"
          param_info = func_info.func_doc.find_param_info(idents[2])
          if param_info != nil
            t = $func_xmldoc_param_template.gsub("PARAM_NAME", param_info.name);
            t.gsub!("DETAIL", param_info.detail);
            $output += t + "\n"
            next
          else
            p "not found param ... " + idents[2]
          end
        end
      end
    end
  end
  
  $output += line_text
end

# BOM を付けて出力
$output = "\xEF\xBB\xBF" + $output
File.open(ARGV[1], "w:utf-8") {|f| f.write $output}

p "[Completed.]"

