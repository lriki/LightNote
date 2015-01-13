#! ruby -Ku
#==============================================================================
# ■ HTML ファイル内のパス変換
#   コマンドライン引数
#     ・対象ファイル名(フルパス), 移動先ファイル名, パスの先頭に付加する文字列
#==============================================================================

$target_ext = [
  ".css",
  ".js",
  ".swf",
  ".html",
  ".png"
]

#==============================================================================
# ▲ 設定ここまで
#==============================================================================

$output = ""

dir_path = File.dirname(ARGV[0]) + "/"
html_file = open(ARGV[0])

while line = html_file.gets do

  # "" 部分
  ary = line.scan(/".*?"/)
  for s in ary
    s.delete!("\"")
    for ext in $target_ext
      if s.index(ext) != nil
        if File.exist?(dir_path + s)
          line.gsub!(s, ARGV[2] + "/" + s)
        end
      end
    end
  end
  
=begin 
  うまく動かなかったけど、トップページは使わないのでこれで・・・
  # '' 部分
  ary = line.scan(/'.*?'/)
  for s in ary
    s.delete!("\"")
    for ext in $target_ext
      if s.index(ext) != nil
        if File.exist?(dir_path + s)
          line.gsub!(s, ARGV[2] + "/" + s)
        end
      end
    end
  end
=end

  $output += line
end

open(ARGV[1], "w") {|f| f.write $output}

p "[ReplacePath finished.]"
