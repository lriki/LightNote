# coding: utf-8
#==============================================================================
# ■ Main_HSPHeader
#==============================================================================
require './Analyzer/FuncLibAnalyzer.rb'

# .as テンプレートファイル
$header_template_filepath = "HSPTemplate/AS_Header.txt"
$footer_template_filepath = "HSPTemplate/AS_Footer.txt"

# 最終出力全体に対して行う文字列置換
$global_replace_strings = [
  ["\"LConfig_SetUserWindowHandle\" var", "\"LConfig_SetUserWindowHandle\" int"],
  ["\"LNote_End\"",                       "onexit \"LNote_End\""]
]

# 型名変換 (enum 等、ここにないものはすべて int に置き換えられる)
def replace_type(cpp_type)
  type_table = [
    ["int",         "int"],
    ["float",       "float"],
    ["bool",        "int"],
    ["lnChar*",     "str"],
    ["TCHAR*" ,     "str"],
    ["void*",       "var"],
    ["lnHandle*",   "var"],   # 上から順に include? で判定しているので、* のあるものを先にしておく
    ["lnHandle",    "int"],
    ["LNResult",    "int"],
    ["lnVector3*",  "var"],
    ["void",        "void"]   # 戻り値の誤判定防止用
  ]
  for pair in type_table
    if cpp_type.include?(pair[0])
      return pair[1]
    end
  end
  return nil  # 見つからなかった
end

#==============================================================================
# ▲ 設定ここまで
#==============================================================================

#--------------------------------------------------------------
# C++ ヘッダ解析
analyzer = FuncLibAnalyzer.new
analyzer.analyze($target_files)

#--------------------------------------------------------------
# ヘッダ
file = File.open($header_template_filepath)
$output = file.read
$output += "\n"

#--------------------------------------------------------------
# #func
for func in analyzer.func_info_list
  arg_list = func.func_decl.arg_list

  # 前処理
  doc_option_info = func.get_doc_option_info("hsp")
  next if doc_option_info.enabled == false                    # 無効かどうか
  next if func.func_opt.hsp.enable == false                   # 無効かどうか
  func_name    = doc_option_info.func_name                    # 出力する関数名
  override_str = (doc_option_info.overridden) ? "__" : " _"   # 関数名プレフィックス
  override_str = (func.func_opt.hsp.override) ? "__" : " _"   # 関数名プレフィックス

  # "#func"
  line_text = "#func " + override_str + func_name + " " + "\"" + func.func_name + "\""
  
  # "#func" の引数部分
  for i in 0...arg_list.size
    line_text += " " if i == 0
    line_text += ", " if i != 0
    
    # 型置換
    type = replace_type(arg_list[i].type)
    type = "int" if type == nil # 変換リストに見つからなければ int 

    line_text += type
  end
  
  # #define
  line_text += "\n"
  line_text += "#define " + func_name
  
  # #define 引数部分
  if arg_list.size > 0
    line_text += "("
    for i in 0...arg_list.size
      line_text += ", " if i != 0
      line_text += "%" + (i + 1).to_s
      line_text += "=" + arg_list[i].default if arg_list[i].default != nil
    end
    line_text += ")"
  end
  
  # #define 定義部分 - 命令名
  line_text += " _" + func_name
  
  # #define 定義部分 - 引数
  if arg_list.size > 0
    line_text += " "
    for i in 0...arg_list.size
      line_text += ", " if i != 0
      line_text += "%" + (i + 1).to_s
    end
  end
  
  line_text += "\n"
  $output += line_text
end
$output += "\n"

#--------------------------------------------------------------
# enum 展開
enum_analyzer = EnumAnalyzer.new
for file_path in $enum_files
  enum_analyzer.analyze($lnote_root + file_path)
end

for enum_decl in enum_analyzer.enum_decl_list
  for member in enum_decl.member_list
    $output += "#const " + member.name + " " + member.value + "\n"
  end
end
$output += "\n"

#--------------------------------------------------------------
# グローバルの置換処理
for pair in $global_replace_strings
  $output.gsub!(pair[0], pair[1])
end

#--------------------------------------------------------------
# フッタ
file = File.open($footer_template_filepath)
$output += file.read

#--------------------------------------------------------------
# 出力
open("lnote.as", "w") {|f| f.write $output}

p "[Completed.]"
