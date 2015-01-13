# coding: utf-8
#==============================================================================
# ■ EnumAnalyzer
#------------------------------------------------------------------------------
#   この後の処理に必要な文字列を抽出する。
#   →enum 型名と、定数名～ ///< ～$
#==============================================================================
require_relative './../Config.rb'

# ヘッダコメントと宣言文のデータクラス
class EnumMember
  attr_reader :name       # 定数名
  attr_reader :value      # 値 (string)
  attr_reader :comment    # コメント(nil or string)
  def initialize(n, v, c)
    @name = n
    @value = v
    @comment = c
  end
end

# ヘッダコメントと宣言文のデータクラス
class EnumDecl
  attr_accessor :type_name        # 型名
  attr_accessor :member_list      # メンバ (行頭～名前, ～ ///< ～ $。前後の空白は除去済み)
  def initialize(n)
    @type_name = n
    @member_list = []
  end
end

class EnumAnalyzer
  #----------------------------------------------------------------------------
  # 
  #----------------------------------------------------------------------------
  attr_reader :enum_decl_list   # EnumDecl の配列
  #----------------------------------------------------------------------------
  # 
  #----------------------------------------------------------------------------
  def initialize
    @enum_decl_list = []
  end
  #----------------------------------------------------------------------------
  # EnumDecl の検索
  #----------------------------------------------------------------------------
  def find_enum(name)
    return enum_decl_list.find{|e| e.type_name == name }
  end
  #----------------------------------------------------------------------------
  # 
  #----------------------------------------------------------------------------
  def analyze(file_path)
    @step = 0
    @current_enum_text = ""
    # 1 行ずつ読んでいく
    file = open(file_path, "r:" + $target_files_encoding)
    while line_text = file.gets do
      case @step
        when 0
          step0_find_enum(line_text)
        when 1
          step0_find_right_brace(line_text)
      end
    end
  end
  #----------------------------------------------------------------------------
  # 
  #----------------------------------------------------------------------------
  def step0_find_enum(line_text)
    if line_text.include?("enum")
      @step = 1
      @current_enum_text = line_text
    end
  end
  #----------------------------------------------------------------------------
  # 
  #----------------------------------------------------------------------------
  def step0_find_right_brace(line_text)
    @current_enum_text += line_text
    if line_text.include?("}") and line_text.include?(";")
      extract
      @step = 0
      @current_enum_text = ""
    end
  end
  #----------------------------------------------------------------------------
  # 
  #----------------------------------------------------------------------------
  def extract
    # "enum 型名 {" を取り出す
    @current_enum_text =~ /enum\s*.*\s*\{/
    
    # 型名を取り出す
    type_name = $&
    type_name.gsub!("enum", "")
    type_name.gsub!("{", "")
    type_name.gsub!("\n", "")
    type_name.gsub!("_", "")    # タグ名の先頭の _
    type_name.strip!
    
    
    # {～} を取り出す
    @current_enum_text =~ /\{.*\}/m
    members = $&
    members.slice!(0)
    members.slice!(members.length - 1)
    members.strip!
    
    last_value = 0
    enum_decl = EnumDecl.new(type_name)
    
    # 行単位に分解
    members_list = members.split("\n")
    for line in members_list
      # "///<" の前後に分割
      if line =~ /\/\/\/<\s*/
        member_decl = $`
        comment = $'
      else
        member_decl = line
        comment = nil
      end
      
      # 空白と "," を削除
      member_decl.delete!("\t\s")
      member_decl.gsub!(",", "")
      
      # 空行の場合は無視
      if member_decl.empty?
        next
      end
      
      member_value = nil
      # "=" がある
      if member_decl =~ /=/
        member_name = $`
        member_value = $'
        last_value = $'.hex
      else
        member_name = member_decl
      end
      
      # 追加
      unless member_name =~ /MAX$/
        
        enum_decl.member_list.push(EnumMember.new(member_name, (member_value != nil) ? member_value : last_value.to_s, comment))
      end
      last_value += 1
    end
    
    # 追加
    @enum_decl_list.push(enum_decl)
  end
end

#==============================================================================
#
#==============================================================================
