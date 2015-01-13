# coding: utf-8
#==============================================================================
# ■ CSComment
#------------------------------------------------------------------------------
#   コマンドの引数：
#     ARGV[0] … 元ファイル
#     ARGV[1] … 出力先ファイル
#   書式：
#     //#[関数名].[brief | param | return | par].[param の場合、引数名]
#==============================================================================
require_relative './Analyzer/FuncLibAnalyzer.rb'
require_relative './Analyzer/CSUtils.rb'

# <summary> コメントのテンプレート
$func_xmldoc_summary_template = "        /// <summary>\n        /// SUMMARY\n        /// </summary>"

# <param> コメントのテンプレート
$func_xmldoc_param_template   = "        /// <param name=\"PARAM_NAME\">DETAIL</param>"

# <returns> コメントのテンプレート
$func_xmldoc_return_template  = "        /// <returns>DETAIL</returns>"

# <remarks> コメントのテンプレート
$func_xmldoc_remarks_template  = "        /// <remarks>\nDETAIL\n        /// </remarks>"

#==============================================================================
# ▲ 設定ここまで
#==============================================================================

# C++ ヘッダ解析
analyzer = FuncLibAnalyzer.new
analyzer.analyze($target_files)

# enum 解析
$enum_analyzer = EnumAnalyzer.new
for file_path in $enum_files
  $enum_analyzer.analyze($lnote_root + file_path)
end

# C# ソースを 1 行ずつ読んでいく
$output = ""
$class_body_output = ""
file = File.open(ARGV[0], "r:utf-8")
while line_text = file.gets do
  if line_text.include?("//#")
    # 空白削除 > "//" #削除 > "." で分割
    idents = line_text.strip.delete("//#").split(".")

    # 関数検索
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
          # 行先頭に /// を付けていく
          t = func_info.func_doc.detail.chomp # 末尾の改行を削除
          t = t.gsub("\n", "\n        /// ")
          t = "        /// " + t
          $output += $func_xmldoc_remarks_template.gsub("DETAIL", t) + "\n";
        end
      else
        raise "unknown command ... " + idents[1]
      end
    end
    
  # 特殊コメント行ではない。そのまま出力
  else
    $output += line_text
  end
end

# BOM を付けて出力
$output = "\xEF\xBB\xBF" + $output
File.open(ARGV[1], "w:utf-8") {|f| f.write $output}

p "[Completed.]"
