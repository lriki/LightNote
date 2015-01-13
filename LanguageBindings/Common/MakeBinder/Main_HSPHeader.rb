# coding: utf-8
#==============================================================================
# �� Main_HSPHeader
#==============================================================================
require './Analyzer/FuncLibAnalyzer.rb'

# .as �e���v���[�g�t�@�C��
$header_template_filepath = "HSPTemplate/AS_Header.txt"
$footer_template_filepath = "HSPTemplate/AS_Footer.txt"

# �ŏI�o�͑S�̂ɑ΂��čs��������u��
$global_replace_strings = [
  ["\"LConfig_SetUserWindowHandle\" var", "\"LConfig_SetUserWindowHandle\" int"],
  ["\"LNote_End\"",                       "onexit \"LNote_End\""]
]

# �^���ϊ� (enum ���A�����ɂȂ����̂͂��ׂ� int �ɒu����������)
def replace_type(cpp_type)
  type_table = [
    ["int",         "int"],
    ["float",       "float"],
    ["bool",        "int"],
    ["lnChar*",     "str"],
    ["TCHAR*" ,     "str"],
    ["void*",       "var"],
    ["lnHandle*",   "var"],   # �ォ�珇�� include? �Ŕ��肵�Ă���̂ŁA* �̂�����̂��ɂ��Ă���
    ["lnHandle",    "int"],
    ["LNResult",    "int"],
    ["lnVector3*",  "var"],
    ["void",        "void"]   # �߂�l�̌딻��h�~�p
  ]
  for pair in type_table
    if cpp_type.include?(pair[0])
      return pair[1]
    end
  end
  return nil  # ������Ȃ�����
end

#==============================================================================
# �� �ݒ肱���܂�
#==============================================================================

#--------------------------------------------------------------
# C++ �w�b�_���
analyzer = FuncLibAnalyzer.new
analyzer.analyze($target_files)

#--------------------------------------------------------------
# �w�b�_
file = File.open($header_template_filepath)
$output = file.read
$output += "\n"

#--------------------------------------------------------------
# #func
for func in analyzer.func_info_list
  arg_list = func.func_decl.arg_list

  # �O����
  doc_option_info = func.get_doc_option_info("hsp")
  next if doc_option_info.enabled == false                    # �������ǂ���
  next if func.func_opt.hsp.enable == false                   # �������ǂ���
  func_name    = doc_option_info.func_name                    # �o�͂���֐���
  override_str = (doc_option_info.overridden) ? "__" : " _"   # �֐����v���t�B�b�N�X
  override_str = (func.func_opt.hsp.override) ? "__" : " _"   # �֐����v���t�B�b�N�X

  # "#func"
  line_text = "#func " + override_str + func_name + " " + "\"" + func.func_name + "\""
  
  # "#func" �̈�������
  for i in 0...arg_list.size
    line_text += " " if i == 0
    line_text += ", " if i != 0
    
    # �^�u��
    type = replace_type(arg_list[i].type)
    type = "int" if type == nil # �ϊ����X�g�Ɍ�����Ȃ���� int 

    line_text += type
  end
  
  # #define
  line_text += "\n"
  line_text += "#define " + func_name
  
  # #define ��������
  if arg_list.size > 0
    line_text += "("
    for i in 0...arg_list.size
      line_text += ", " if i != 0
      line_text += "%" + (i + 1).to_s
      line_text += "=" + arg_list[i].default if arg_list[i].default != nil
    end
    line_text += ")"
  end
  
  # #define ��`���� - ���ߖ�
  line_text += " _" + func_name
  
  # #define ��`���� - ����
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
# enum �W�J
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
# �O���[�o���̒u������
for pair in $global_replace_strings
  $output.gsub!(pair[0], pair[1])
end

#--------------------------------------------------------------
# �t�b�^
file = File.open($footer_template_filepath)
$output += file.read

#--------------------------------------------------------------
# �o��
open("lnote.as", "w") {|f| f.write $output}

p "[Completed.]"
