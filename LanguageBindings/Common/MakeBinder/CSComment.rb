# coding: utf-8
#==============================================================================
# �� CSComment
#------------------------------------------------------------------------------
#   �R�}���h�̈����F
#     ARGV[0] �c ���t�@�C��
#     ARGV[1] �c �o�͐�t�@�C��
#   �����F
#     //#[�֐���].[brief | param | return | par].[param �̏ꍇ�A������]
#==============================================================================
require_relative './Analyzer/FuncLibAnalyzer.rb'
require_relative './Analyzer/CSUtils.rb'

# <summary> �R�����g�̃e���v���[�g
$func_xmldoc_summary_template = "        /// <summary>\n        /// SUMMARY\n        /// </summary>"

# <param> �R�����g�̃e���v���[�g
$func_xmldoc_param_template   = "        /// <param name=\"PARAM_NAME\">DETAIL</param>"

# <returns> �R�����g�̃e���v���[�g
$func_xmldoc_return_template  = "        /// <returns>DETAIL</returns>"

# <remarks> �R�����g�̃e���v���[�g
$func_xmldoc_remarks_template  = "        /// <remarks>\nDETAIL\n        /// </remarks>"

#==============================================================================
# �� �ݒ肱���܂�
#==============================================================================

# C++ �w�b�_���
analyzer = FuncLibAnalyzer.new
analyzer.analyze($target_files)

# enum ���
$enum_analyzer = EnumAnalyzer.new
for file_path in $enum_files
  $enum_analyzer.analyze($lnote_root + file_path)
end

# C# �\�[�X�� 1 �s���ǂ�ł���
$output = ""
$class_body_output = ""
file = File.open(ARGV[0], "r:utf-8")
while line_text = file.gets do
  if line_text.include?("//#")
    # �󔒍폜 > "//" #�폜 > "." �ŕ���
    idents = line_text.strip.delete("//#").split(".")

    # �֐�����
    func_info = analyzer.find_func_info(idents[0])
    if func_info == nil
      p "not found func ... " + idents[0]
    else
      # [brief]
      if idents[1] == "brief"
        $output += $func_xmldoc_summary_template.gsub("SUMMARY",func_info.func_doc.summary) + "\n";
        next
      # [param]
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
      # [return]
      elsif idents[1] == "return"
        if func_info.func_doc.return_detail != nil
          $output += $func_xmldoc_return_template.gsub("DETAIL", func_info.func_doc.return_detail) + "\n";
        end
      # [par]
      elsif idents[1] == "par"
        if func_info.func_doc.detail != nil
          # �s�擪�� /// ��t���Ă���
          t = func_info.func_doc.detail.chomp # �����̉��s���폜
          t = t.gsub("\n", "\n        /// ")
          t = "        /// " + t
          $output += $func_xmldoc_remarks_template.gsub("DETAIL", t) + "\n";
        end
      else
        raise "unknown command ... " + idents[1]
      end
    end
    
  # ����R�����g�s�ł͂Ȃ��B���̂܂܏o��
  else
    $output += line_text
  end
end

# BOM ��t���ďo��
$output = "\xEF\xBB\xBF" + $output
File.open(ARGV[1], "w:utf-8") {|f| f.write $output}

p "[Completed.]"
