# coding: utf-8
#==============================================================================
# ■ C# Main
#==============================================================================
require './Config.rb'
require './Analyzer/FuncHeaderFinder.rb'
require './Analyzer/EnumAnalyzer.rb'
require './Analyzer/FuncHeaderAnalyzer.rb'
require './Analyzer/FuncDeclAnalyzer.rb'
require './Analyzer/CSUtils.rb'

# ソースファイルのテンプレート
$source_template_file = "CSTemplate/Source.txt"


$dll_import_tempalte = "[DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]"
$func_decl_tempalte = "public extern static RETURN_TYPE FUNC_NAME(ARGS);"
$func_arg_tempalte = "TYPE NAME"
$func_arg_with_default_tempalte = "TYPE NAME = DEFAULT"

# コメントテンプレート
$func_xmldoc_summary_template = "        /// <summary>\n        /// SUMMARY\n        /// </summary>"
$func_xmldoc_param_template = "        /// <param name=\"PARAM_NAME\">DETAIL</param>"
$func_xmldoc_return_template = "        /// <returns>DETAIL</returns>"
$enum_member_doc_template = "        /// <summary>\n        /// SUMMARY\n        /// </summary>"

# 小文字化しない enum 定数名
$enum_not_captal_word_list = [
  "XYZ", "XZY", "YXZ", "YZX", "ZXY", "ZYX",
  "BGM", "BGS", "ME",
  "A8R8G8B8", "X8R8G8B8", "A16B16G16R16F", "A32B32G32R32F", "D24S8"
]

#==============================================================================
# ▲ 設定ここまで
#==============================================================================

$output = ""

func_header_finder = FuncHeaderFinder.new
$enum_analyzer = EnumAnalyzer.new






# 全ファイル解析
for file_path in $target_files
  func_header_finder.analyze($lnote_root + file_path)
end
# enum 解析
for file_path in $enum_files
  $enum_analyzer.analyze($lnote_root + file_path)
end

#--------------------------------------------------------------
# 関数ごとの詳細解析
for func_doc in func_header_finder.func_text_list
  func_doc_analyzer = FuncHeaderAnalyzer.new
  func_doc_analyzer.analyze(func_doc.doc_text, func_doc.doc_opt_text)
  func_decl_analyzer = FuncDeclAnalyzer.new
  func_decl_analyzer.analyze(func_doc.decl_text)
  
  # DLLImport・型名・関数名
  func_text = $func_decl_tempalte.gsub("RETURN_TYPE", convert_type_to_cs_basic(func_decl_analyzer.return_type))
  func_text = func_text.gsub("FUNC_NAME", func_decl_analyzer.func_name)
  func_text = "        " + $dll_import_tempalte + "\n        " + func_text;
  
  # 引数リスト
  args_text = ""
  for arg in func_decl_analyzer.arg_list
    # 2つ目以降の引数なら , を追加
    if args_text != ""  
      args_text += ", "
    end
    
    # 型名
    type_name = convert_param_type_to_cs_for_api(arg.type)#convert_type_to_cs(arg.type)
    
    # デフォルト引数の有無
    if arg.default == nil
      args_text += $func_arg_tempalte.gsub("TYPE", type_name).gsub("NAME", arg.name)
    else
      args_text += $func_arg_tempalte.gsub("TYPE", type_name).gsub("NAME", arg.name).gsub("DEFAULT", arg.default)
    end
  end
  func_text = func_text.gsub("ARGS", args_text)
  
  # XML コメント
  # <summary>
  comment = $func_xmldoc_summary_template.gsub("SUMMARY", func_doc_analyzer.summary) + "\n"
  # <param>
  for par in func_doc_analyzer.param_pars
    t = $func_xmldoc_param_template.gsub("PARAM_NAME", par.name)
    t.gsub!("DETAIL", par.detail)
    comment += t + "\n"
  end
  # <returns>
  if func_doc_analyzer.return_detail != nil and func_doc_analyzer.return_detail != ""
    comment += $func_xmldoc_return_template.gsub("DETAIL", func_doc_analyzer.return_detail) + "\n"
  end
  
  $output += comment + func_text + "\n\n"
end

#--------------------------------------------------------------
# enum 展開
enum_decl_list_text = ""
indent = "    "
for enum_decl in $enum_analyzer.enum_decl_list
  type_name = convert_type_to_cs_basic(enum_decl.type_name)

  enum_decl_text  = indent + "public enum " + type_name + "\n"
  enum_decl_text += indent + "{\n"
  
  flags_type = false  # [Flags] 属性か
  
  for member in enum_decl.member_list
    # XML コメント
    if member.comment != nil
      enum_decl_text += $enum_member_doc_template.gsub("SUMMARY", member.comment) + "\n"
    else
      enum_decl_text += "\n"
    end
    
    # メンバ
    name = member.name
    if name == "LN_OK"
      # これだけはフォーマットが違う
      name = "OK";
    else
      # _ の次の単語の先頭のみを大文字にする
      idents = name.split("_")
      name = ""
      for i in 2...idents.size
        if $enum_not_captal_word_list.include?(idents[i])
          name += idents[i]             # 先頭大文字、以降小文字化しない
        else
          name += idents[i].capitalize  # 先頭大文字、以降小文字化する
        end
      end
    end
    enum_decl_text += (indent * 2) + name + " = " + member.value + ", \n\n"
    
    # [Flags]
    if member.value[0] == "0" and member.value[1] == "x"
      flags_type = true
    end
  end
  
  enum_decl_text += indent + "}"
  
  enum_decl_list_text += indent + "[Flags]\n" if flags_type
  enum_decl_list_text += enum_decl_text + "\n\n"
end
=begin
$output = ""
# テスト出力
for doc_decl in func_header_finder.func_text_list
  p doc_decl.doc_text
  p doc_decl.decl_text
  p "\n"
  $output += doc_decl.doc_opt_text
end


File.open("test.txt", "w:windows-31j") {|f| f.write $output}
=end





source_template_file = File.open($source_template_file)
source_template_text = source_template_file.read
$output = source_template_text.gsub("API_LIST", $output)
$output = $output.gsub("ENUM_LIST", enum_decl_list_text)

#$output = file.read

$output = "\xEF\xBB\xBF" + $output #write BOM
File.open("LN.cs", "w:utf-8") {|f| f.write $output}


p "[Completed.]"

#==============================================================================
#
#==============================================================================
