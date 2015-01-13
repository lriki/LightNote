# coding: utf-8
#==============================================================================
# ■ FuncHeaderAnalyzer
#------------------------------------------------------------------------------
#   FuncHeaderFinder で抽出した内容を分解 (ヘッダコメントのみ)
#==============================================================================
require_relative './../Config.rb'

# @param
class ParamInfo
  #----------------------------------------------------------------------------
  # 
  #----------------------------------------------------------------------------
  attr_accessor :io_type      # 入出力の種類 "[in]" "[out]" "[in,out]"
  attr_accessor :name         # 引数名
  attr_accessor :detail       # 説明 (':' を含む)
  attr_accessor :arg_texts    # @arg 文字列配列 "NAME : DETAIL"
  #----------------------------------------------------------------------------
  # line_text : @param を含む行
  #----------------------------------------------------------------------------
  def initialize(line_text)
    @io_type = ""
    @name = ""
    @detail = ""
    @arg_texts = []
    
    if line_text =~ /\@param/
      $' =~ /\[.+\]/
      @io_type = $&    # $& = マッチした箇所
      # "x, y, z" の暫定対策。':' の前までを引数名とみなす
      if $'.include?(":")
        $' =~ /:.+/
        @name = $`.strip
        @detail = $&.strip
      else
        $' =~ /\s+\S+\s+/
        @name = $&
        @name.strip!  # 前後のスペースを削除
        @detail = $'
      end
    end
  end
  #----------------------------------------------------------------------------
  # line_text : @arg を含む行   (AA : 説明等、@arg の後ろすべてが入る)
  #----------------------------------------------------------------------------
  def analyze_arg_line(line_text)
    if line_text =~ /\@arg\s+/
      @arg_texts.push($'.strip)
    end
  end
end

# オプション引数
class OptArgInfo
  attr_accessor :name         # 引数名
  attr_accessor :value        # 値
  def initialize(name, value)
    @name = name
    @value = value
  end
end

class FuncHeaderAnalyzer
  #----------------------------------------------------------------------------
  # 
  #----------------------------------------------------------------------------
  attr_accessor :summary        # 概要  @brief
  attr_accessor :param_pars     # 引数詳細の配列 (ParamPar クラスの配列)
  attr_accessor :return_detail  # return 説明
  attr_accessor :retvals        # @retval の配列 ("値 : 説明" 等)
  attr_accessor :detail         # 詳細 (@par)
  attr_accessor :option_map     # オプション引数配列のハッシュマップ (OptArgInfo クラス配列のマップ。キーは言語名。Config.rb で定義)
  #----------------------------------------------------------------------------
  # 初期化
  #----------------------------------------------------------------------------
  def initialize
    @summary = ""
    @param_pars = []
    @return_detail = nil
    @retvals = []
    @detail = ""
    @option_args = []
    @cur_param_par = nil
    @func_detail_analyzing = false
    @return_detail_analyzing = false
    @option_map = {}
    for name in $option_arg_lang_names
      @option_map[name] = []
    end
  end
  #----------------------------------------------------------------------------
  # 
  #----------------------------------------------------------------------------
  def find_param_info(name)
    return @param_pars.find{|param| param.name == name }
  end
  #----------------------------------------------------------------------------
  # 
  #----------------------------------------------------------------------------
  def analyze(doc_comment, doc_opt_text)
    @step = 1
    # 1行ずつ
    doc_comment_lines = doc_comment.split("\n")
    for line_text in doc_comment_lines
      case @step
        when 1
          step1_analyze_doc(line_text)
        #when 2
          #step2_analyze_lang_option(line_text)
      end
    end
    
    end_param_analyze # 最後の掃出し
    end_par_analyze   # 最後の掃出し
    
    # オプション引数
    analyze_lang_option(doc_opt_text)
  end
  #----------------------------------------------------------------------------
  # @param (と、それに付属する @arg) 解析終了
  #----------------------------------------------------------------------------
  def end_param_analyze
    return if @cur_param_par == nil
    @param_pars.push(@cur_param_par)
    @cur_param_par = nil
  end
  #----------------------------------------------------------------------------
  # @par 解析終了
  #----------------------------------------------------------------------------
  def end_par_analyze
    @func_detail_analyzing = false
    @return_detail_analyzing = false
  end
  #----------------------------------------------------------------------------
  # step1 doxy コメント解析
  #----------------------------------------------------------------------------
  def step1_analyze_doc(line_text)
    # doc 終端  # 現状ここに入ることはないはず
    if line_text =~ /\s+\/{2}\*\//
      @step = 2
      return
    end
    # @brief
    if line_text =~ /\@brief\s+/
      @summary = $' # 最後にマッチした箇所の後
      return
    end
    # @param
    if line_text.include?("@param")
      end_param_analyze
      end_par_analyze
      @cur_param_par = ParamInfo.new(line_text)
      #@cur_func_doc.param_pars.push(@cur_param_par)
      return
    end
    # @arg
    if line_text.include?("@arg")
      end_par_analyze
      @cur_param_par.analyze_arg_line(line_text)
      return
    end
    # @return
    if line_text =~ /\@return\s+/
      end_param_analyze
      end_par_analyze
      @return_detail = $'
      @return_detail_analyzing = true
      return
    end
    # @retval
    if line_text =~ /\@retval\s+/
      end_param_analyze
      end_par_analyze
      @retvals.push($'.delete("\r\n")) # 改行を削除して追加
    end
    # @par
    if line_text.include?("@par")#line_text =~ /\@par\s/
      end_param_analyze
      @func_detail_analyzing = true
      return
    end
    # @par 解析中
    if @func_detail_analyzing
      @detail += line_text.gsub("<br>", "\n") + "\n" # <br> は改行に変換
      return
    end
    # @return 解析中
    if @return_detail_analyzing
      end_param_analyze
      line_text =~ /\/\/\s*/
      if $' != nil
        @return_detail += $'
      end
      return
    end
  end
  #--------------------------------------------------------------------------
  # 言語別オプション解析
  #   ["disable"] false の場合、disable
  #   doc_opt_text : ヘッダコメントオプション ('['～']'。[] は削除してある)
  #--------------------------------------------------------------------------
  def analyze_lang_option(doc_opt_text)
    #r = []
    #if one_line_str =~ /\[.+\]/
    #  lang_texts = $&.delete("[]").split(",")
      lang_texts = doc_opt_text.split(",")
      for text in lang_texts
        args = text.delete("{}").split(" ")
        
        r = []
        for a in args
          pair = a.split("=")
          case pair[0]
            when "disable"
              r.push(OptArgInfo.new("disable", true))
            else
              r.push(OptArgInfo.new(pair[0], pair[1]))
            #when "name"
            #  r.push(OptArg.new("name", pair[1]))
          end
        end
        
        # ひとつもない場合は有効であることを示す値のみセット
        if r.size == 0
          r.push(OptArgInfo.new("disable", false))
        end
        @option_map[args[0]] = r
        
      end
    #end
    # ひとつもない場合は有効であることを示す値のみセット
    #if r.size == 0
    #  r.push(OptArgInfo.new("disable", false))
    #end
    #@option_args = r
    @step = 1
    
    
  end
end

#==============================================================================
#
#==============================================================================
