# coding: utf-8
#==============================================================================
# ■ HSP ヘルプファイルを作る
#------------------------------------------------------------------------------
#==============================================================================
require './Analyzer/FuncLibAnalyzer.rb'

# グループ
$class_group = {
  "LException"        => "例外処理",
  "LConfig"           => "初期設定",
  "LFileIO"           => "ファイル入出力",
  "LInFile"           => "ファイル入出力",
  "LOutFile"          => "ファイル入出力",
  "LAudio"            => "音声機能",
  "LSound"            => "音声機能",
  "L3DSoundListener"  => "音声機能"
}

# ヘッダテンプレート
$hs_header_template_filepath = "HSPTemplate/Help_Header.txt"

# 関数テンプレート
$hs_func_template_filepath = "HSPTemplate/Help_Function.txt"

# 最終出力全体に対して行う文字列置換
$global_replace_strings = [
  ["のポインタ", ""]  # ポインタという表現は無い
]

#==============================================================================
# ▲ 設定ここまで
#==============================================================================

#--------------------------------------------------------------
# C++ ヘッダ解析
analyzer = FuncLibAnalyzer.new
analyzer.analyze($target_files)

#--------------------------------------------------------------
# C++ enum 解析
enum_analyzer = EnumAnalyzer.new
for file_path in $enum_files
  enum_analyzer.analyze($lnote_root + file_path)
end

#--------------------------------------------------------------
# ヘッダ
file = File.open($hs_header_template_filepath, "r:utf-8")
$output = file.read
$output += "\n"

# バージョン
$output.gsub!("_LIBRARY_VERSION_", $lnote_version)

# 日付
day = Time.now
$output.gsub!("_BUILD_TIME_", day.year.to_s + "/" + day.month.to_s + "/" + day.day.to_s)

#--------------------------------------------------------------
# 落とし込み
file = File.open($hs_func_template_filepath, "r:utf-8")
$func_template = file.read

for func in analyzer.func_info_list
  doc = func.func_doc
  
  # 前処理
  doc_option_info = func.get_doc_option_info("hsp")
  next if doc_option_info.enabled == false                    # 無効かどうか
  func_name    = doc_option_info.func_name                    # 出力する関数名
  
  #----------------------------
  # 基本
  func_tmpl = $func_template.dup
  func_tmpl.sub!("_NAME_", func_name)
  func_tmpl.sub!("_BRIEF_", doc.summary)
  func_tmpl.sub!("_INST_", doc.detail)
  func_tmpl.sub!("_HREF_", "")
  
  #----------------------------
  # グループ
  func_name =~ /^.+_/              # '_' の前の部分からグループを特定する
  class_name = $&.delete("_")
  group = $class_group[class_name]
  if group != nil
    func_tmpl.sub!("_GROUP_", group)
  else
    func_tmpl.sub!("_GROUP_", "")
  end
  
  #----------------------------
  # 引数リスト
  arg_list = ""
  for i in 0...doc.param_pars.size
    arg_list += ", " if i != 0
    arg_list += doc.param_pars[i].name
  end
  func_tmpl.sub!("_PRM_LIST_", arg_list)
  
  #----------------------------
  # 引数詳細まで(:)のインデント量を調べる
  arg_detail = ""
  io_space_count = 0    # [in] 等に必要なスペース領域チェック
  for param in doc.param_pars
    io_space_count = param.io_type.length  if param.io_type.length > io_space_count
  end
  io_space_count += 1 # スペース
  name_space_count = 0  # 名前とデフォルト値のスペース領域チェック
  for param in doc.param_pars
    t = param.name.length + 1    # +1 はスペースの分
    default_val = func.find_default_arg_value(param.name)  # デフォルト引数
    if default_val != nil
      t += default_val.length + 2 # +2 は ()
    end
    name_space_count = t if t > name_space_count
  end
  
  #----------------------------
  # 引数ごとの詳細を作る
  for param in doc.param_pars
    # io type
    t = param.io_type
    snum = io_space_count - t.length
    t += " " * snum if snum > 0
    # name
    default_val = func.find_default_arg_value(param.name)  # デフォルト引数
    c = param.name
    c += "(" + default_val + ")" if default_val != nil
    t += c
    snum = name_space_count - c.length
    t += " " * snum if snum > 0
    # detail
    t += param.detail + "\n"
    
    # 引数の型が enum の場合は、EnumAnalyzer からメンバを展開する
    decl_arg = func.func_decl.find_func_arg(param.name)
    if decl_arg != nil
      enum_decl = enum_analyzer.find_enum(decl_arg.type)
      if enum_decl != nil
         for member in enum_decl.member_list
           t += (" " * (io_space_count+name_space_count)) + "    "
           t += "- " + member.name + "\n"
           if member.comment != nil
             t += (" " * (io_space_count+name_space_count)) + "        "
             t += member.comment + "\n"
           end
         end
      end
    end
=begin
    # args
    for a in param.arg_pars
      t += (" " * (io_space_count+name_space_count)) + "    "
      t += a + "\n"
    end
=end
    arg_detail += t
  end
  
  #----------------------------
  # @return
  if doc.return_detail != nil
    arg_detail += "\nstat : "
    lines = doc.return_detail.split("\n")
    for i in 0...lines.size
      if i >= 1
        arg_detail += "\n       " # "stat : " の分の空白
      end
      arg_detail += lines[i]
    end
  end
=begin
  #----------------------------
  # @retval
  unless doc.retvals.empty?
    # 引数が一つもない場合は stat が第一引数として認識されてしまうので、
    # 全角スペースでごまかしてみる
    if doc.param_pars.empty?
      arg_detail += "　"
    end
    
    arg_detail += "\nstat : "
    for line in doc.retvals
      arg_detail += "\n       " + line
    end
  end
=end

  func_tmpl.sub!("_PRM_DETAIL_", arg_detail)
  
  $output += "\n" + func_tmpl + "\n"
end

#--------------------------------------------------------------
# グローバルの置換処理
for pair in $global_replace_strings
  $output.gsub!(pair[0], pair[1])
end

#--------------------------------------------------------------
# 出力

# UTF-8→SJIS
sjis_text = $output.encode("sjis", "utf-8")  
open("lnote.hs", "w:windows-31j") {|f| f.write sjis_text}

p "[Completed.]"
