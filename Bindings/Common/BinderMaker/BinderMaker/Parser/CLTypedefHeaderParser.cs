using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Sprache;

namespace BinderMaker.Parser
{
    /// <summary>
    /// enum�Astruct�A�֐��|�C���^�̒�`�t�@�C�� (�w�b�_�t�@�C��) ����͂���
    /// </summary>
    class CLTypedefHeaderParser
    {
        #region struct

        // struct �����o
        private static readonly Parser<CLStructMember> StructMember =
            from type       in ParserUtils.TypeName.GenericToken()      // �^��
            from name       in ParserUtils.Identifier.GenericToken()    // �����o��
            from colon      in Parse.Char(';').GenericToken()           // ����
            from comment    in ParserUtils.DoxyLineComment2.Or(Parse.Return(""))     // �R�����g�������o����(opt)
            select new CLStructMember(type, name, comment);
        
        // struct ��`
        private static readonly Parser<CLStruct> StructDecl =
            from comment    in ParserUtils.DoxyLineComment1              // �R�����g
            from start      in Parse.String("struct").GenericToken()
            from name       in ParserUtils.Identifier.GenericToken()    // ���O
            from lbrace     in Parse.Char('{').GenericToken()
            from members    in StructMember.Many()                        // �����o
            from rbrace     in Parse.Char('}').GenericToken()
            from end        in Parse.Char(';').GenericToken()
            select new CLStruct(comment, name, members);

        #endregion

        #region enum

        // enum �����o�̒萔��`
        private static readonly Parser<string> EnumMemberValue =
            from equal      in Parse.Char('=').GenericToken()
            from value      in ParserUtils.IdentifierOrNumeric.GenericToken()
            select value;

        // enum �����o
        private static readonly Parser<CLEnumMember> EnumMember =
            from name       in ParserUtils.Identifier.GenericToken()
            from value      in EnumMemberValue.Or(Parse.Return(""))     // �萔�� opt
            from comma      in Parse.Char(',').GenericToken()           // �I�[,
            from comment    in ParserUtils.DoxyLineComment2.Or(Parse.Return(""))     // �R�����g�������o����(opt)
            select new CLEnumMember(name, value, comment);

        // enum ��`
        private static readonly Parser<CLEntity> EnumDecl =
            from comment    in ParserUtils.DoxyLineComment1                          // �R�����g
            from start      in Parse.String("enum").GenericToken()
            from name       in ParserUtils.Identifier.GenericToken()
            from lbrace     in Parse.Char('{').GenericToken()
            from members    in EnumMember.Many()
            from rbrace     in Parse.Char('}').GenericToken()
            from end        in Parse.Char(';').GenericToken()
            select new CLEnum(comment, name, members);

        #endregion

        #region �֐��|�C���^

        // �֐��|�C���^
        private static readonly Parser<CLEntity> FuncPointerDecl =
            from doc        in CLAPIHeaderParser.DocumentComment        // �R�����g
            from start      in Parse.String("typedef").GenericToken()
            from type       in ParserUtils.TypeName.GenericToken()      // �߂�l�^
            from lparen1    in Parse.Char('(').GenericToken()
            from ast        in Parse.Char('*').GenericToken()
            from name       in ParserUtils.Identifier.GenericToken()    // ���O
            from rparen1    in Parse.Char(')').GenericToken()
            from lparen     in Parse.Char('(').GenericToken()
            from params1    in CLAPIHeaderParser.FuncParamDecls.Or(Parse.Return<IEnumerable<CLParam>>(null))    // �������X�gopt
            from rparen     in Parse.Char(')').GenericToken()
            from end1       in Parse.Char(';').GenericToken()
            select new CLDelegate(doc, type, name, params1);

        #endregion

        #region �t���[�����[�N

        // ��`�v�f
        private static readonly Parser<CLEntity> DeclContent =
                StructDecl
            .Or(EnumDecl)
            .Or(FuncPointerDecl);

        // ���[�g�v�f
        private static readonly Parser<IEnumerable<CLEntity>> CompileUnit =
            from lead       in Parse.AnyChar.Except(Parse.String("/**")).Many()   // �t�@�C���擪����ŏ��̃h�L�������g�R�����g�܂ł�ǂݔ�΂� (/** �͏���Ȃ�)
            from decls      in DeclContent.Many()
            from any        in Parse.AnyChar.Many()     // ��͓K���ȕ���
            select decls;

        #endregion

        #region Methods

        public void Analyze(string filePath)
        {
            string text = System.IO.File.ReadAllText(filePath);
            text = text.Replace("\r\n", "\n");  // ���s�R�[�h�� LF �ɓ���
            var decls = CompileUnit.Parse(text);
            Console.WriteLine(decls);
        }

        #endregion
    }
}
