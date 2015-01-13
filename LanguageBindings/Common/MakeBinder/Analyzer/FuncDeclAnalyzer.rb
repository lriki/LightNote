# coding: utf-8
#==============================================================================
# ■ FuncDeclAnalyzer
#------------------------------------------------------------------------------
#   FuncHeaderFinder で抽出した内容を分解(関数宣言1行分)
#==============================================================================
require_relative './../Config.rb'

class FuncArg
  attr_accessor :name         # 引数名
  attr_accessor :default      # デフォルト引数
  attr_accessor :type         # 型
  attr_accessor :attr_ref     # 値参照タイプ (const 有りポインタ型であるか。C#のref)
  attr_accessor :attr_out     # 出力タイプ (const 無しポインタ型であるか。C#のout)
  def initialize
    @name = ""
    @default = nil
    @type = ""
    @attr_ref = false
    @attr_out = false
  end
  def post_check
    if @type.include?("*")
      if @type.include?("const")
        @attr_ref = true
      else
        @attr_out = true
      end
    end
  end
end

class FuncDeclAnalyzer
  #----------------------------------------------------------------------------
  # 
  #----------------------------------------------------------------------------
  attr_accessor :func_name    # 関数名
  attr_accessor :return_type  # 戻り値の型名
  attr_accessor :arg_list     # 引数リスト (FuncArg の配列)
  attr_accessor :module_name  # モジュール名 (関数名 _ の前の部分)
  attr_accessor :method_name  # メソッド名 (関数名 _ の後ろの部分)
  #----------------------------------------------------------------------------
  #
  #----------------------------------------------------------------------------
  def initialize
    @func_name = ""
    @return_type = ""
    @arg_list = []
    @module_name = ""
    @method_name = ""
  end
  #----------------------------------------------------------------------------
  # FuncArg の検索
  #----------------------------------------------------------------------------
  def find_func_arg(arg_name)
    return @arg_list.find{|arg| arg.name == arg_name }
  end
  #----------------------------------------------------------------------------
  # 
  #----------------------------------------------------------------------------
  def analyze(decl_line)
    unless decl_line.include?(");")
      # 関数終端の ); が見つからなかった (間にスペースがあるなど)
      raise "); termination of the function does not exist.\n" + decl_line
    end
    if decl_line.include?("=")
      # 普通のデフォルト引数文はエラー
      raise "use LNOTE_DEFAULT_ARG the default argument.\n" + decl_line
    end
    
    # (); とその前とに分ける   "void Func" "(int x);"
    decl_line =~ /\(.*\);/
    arg_str = $&
    
    # 関数名・型名 ('('直前のトークンが関数名。それ以前はすべて型名となる)
    $` =~ /\S+$/
    @func_name = $&
    @return_type = $`.strip
    
    # 引数 - 前後の括弧を削除し、分割
    #     LNOTE_DEFAULT_ARG(LN_ROTATIONORDER_XYZ) を残す。
    #arg_str.delete!("()")
    arg_str.slice!("(")
    arg_str.slice!(");")
    args = arg_str.split(",")
    for a in args
      arg_decl = FuncArg.new
      a.strip!
      
=begin
      # '=' で分割 (デフォルト引数があるか？)
      pair = a.split("=")
      arg_decl.name = pair[0]
      if pair.size != 1 # デフォルト引数がある
        arg_decl.default = pair[1]
        arg_decl.default.strip!
      end
=end
      # 'LNOTE_DEFAULT_ARG' の前後で分割 (デフォルト引数があるか？)
      pair = a.split("LNOTE_DEFAULT_ARG")
      arg_decl.name = pair[0]
      if pair.size != 1 # デフォルト引数がある
        arg_decl.default = pair[1].delete("()");  # マクロ前後の ( ) を削除
        arg_decl.default.strip!
      end
      
      # 型部と名前に分ける
      arg_decl.name.strip!
      arg_decl.name =~ /\S+$/
      arg_decl.name = $&
      arg_decl.type = $`.strip
      arg_decl.post_check
      @arg_list.push(arg_decl)
    end
    
    # 関数名からモジュール名を取得 (_ で分割)
    pair = @func_name.split("_")
    @module_name = pair[0]
    @method_name = pair[1]
  end
end

