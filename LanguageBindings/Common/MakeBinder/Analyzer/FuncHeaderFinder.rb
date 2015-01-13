# coding: utf-8
#==============================================================================
# ■ FuncHeaderFinder
#------------------------------------------------------------------------------
#   この後の処理に必要な文字列を抽出する。
#   →関数ヘッダと関数定義を抽出する。
#   ("  ///**" ～ ";" までの内容。行頭にスペースまたはタブが必要なので注意)
#==============================================================================
require_relative './../Config.rb'

# ヘッダコメントと宣言文のデータクラス
class FuncDocDeclText
  attr_reader :doc_text         # 関数ヘッダコメント (行頭の // や /** は削除してある)
  attr_reader :doc_opt_text     # ヘッダコメントオプション ('['～']'。[] は削除してある)
  attr_reader :decl_text        # 関数宣言 (～;。; も含む)
  attr_accessor :opt_text       # オプション。スクリプト部分のみ (/*DECOPT、DECOPTEND*/ は含まない)
  def initialize(doc, opt, decl)
    @doc_text = doc
    @doc_opt_text = opt
    @decl_text = decl
    @opt_text = nil
  end
end

class FuncHeaderFinder
  #----------------------------------------------------------------------------
  # 
  #----------------------------------------------------------------------------
  attr_accessor :func_text_list   # 解析結果の FuncDocDeclText 配列
  attr_accessor :module_text_list # 解析結果の string 配列
  #----------------------------------------------------------------------------
  # 初期化
  #----------------------------------------------------------------------------
  def initialize
    @step = 0
    @current_func_doc = ""
    @current_func_doc_opt = ""
    @current_func_doc_opt_script = ""
    @current_func_doc_decl_text = nil
    @func_text_list = []
    @module_text_list = []
  end
  #----------------------------------------------------------------------------
  # ひとつのファイルを解析し、結果を func_text_list に追加する
  #----------------------------------------------------------------------------
  def analyze(file_path)
    # 1 行ずつ読んでいく
    file = open(file_path, "r:utf-8")
    
    module_seq = 0
    module_text = ""
    
    while line_text = file.gets do
      
      # 前処理置換
      for pair in $prepro_replace
        line_text.gsub!(pair[0], pair[1])
      end
      # 関数ヘッダシーケンス
      case @step
        when 0
          step0_find_func_doc_begin(line_text)
        when 1
          step1_find_func_doc_end(line_text)
        when 2
          step2_find_func_decl_or_opt(line_text)
        when 3
          step3_find_opt_begin(line_text)
        when 4
          step4_find_opt_end(line_text)
      end
      # モジュールヘッダシーケンス
      case module_seq
        when 0
          if line_text =~ /^\/{3}\*{2}/ # 行頭からの 「///**」
            module_text = ""
            module_seq = 1
          end
        when 1
          if line_text =~ /^\/{2}\*{1}\/{1}/
            module_seq = 0
            if module_text.include?("@defgroup")  # 本当に @defgroup が含まれていれば追加
              @module_text_list.push(module_text)
            end
          end
          if line_text =~ /\/\/\s*/ # 行頭の "    // " を取り除く
            module_text += $'
          end
      end
    end
  end
  #----------------------------------------------------------------------------
  # 
  #----------------------------------------------------------------------------
  def step0_find_func_doc_begin(line_text)
    # \s+   :スペースofタブを1回以上
    # \/{3} :/ を3回
    # \*{2} :* を2回
    if line_text =~ /\s+\/{3}\*{2}/
      @step = 1
      # リセット (; に一番近い関数ヘッダを対象とする)
      @current_func_doc = ""
    end
  end
  #----------------------------------------------------------------------------
  # 
  #----------------------------------------------------------------------------
  def step1_find_func_doc_end(line_text)
    # step0_find_func_doc_begin と同じ条件。誤認識を防ぐ
    if line_text =~ /\s+\/{3}\*{2}/
      @step = 1
      @current_func_doc = ""
      return
    end
    # \s+   :スペースofタブを1回以上
    # \/{2} :/ を2回
    # \*{1} :* を1回
    # \/{1} :/ を1回
    if line_text =~ /\s+\/{2}\*{1}\/{1}/
      # 関数ヘッダ確定。宣言の検索へ
      @step = 2
      return
    end
    # 関数ヘッダ解析中。行頭の "    // " を取り除く
    if line_text =~ /\/\/\s*/
      @current_func_doc += $'
    end
  end
  #----------------------------------------------------------------------------
  # 
  #----------------------------------------------------------------------------
  def step2_find_func_decl_or_opt(line_text)
    # 関数宣言行の場合
    if line_text =~ /\s+.*\(.*\);/
      # 先頭と末尾の空白を取り除いて確定
      @current_func_doc_decl_text = FuncDocDeclText.new(@current_func_doc, @current_func_doc_opt, line_text.strip)
      @func_text_list.push(@current_func_doc_decl_text)
      @current_func_doc = ""
      @current_func_doc_opt = ""
      @current_func_doc_opt_script = ""
      @step = 3
    end
    # 関数ヘッダ終端の、ヘッダコメントオプション
    if line_text =~ /\[.+\]\s*$/
      @current_func_doc_opt = $&.strip.delete("[]")
    end
  end
  #----------------------------------------------------------------------------
  # 
  #----------------------------------------------------------------------------
  def step3_find_opt_begin(line_text)
    # //---- (次の関数ヘッダの先頭)
    if line_text.include?("/*DECOPT")
      @step = 4
    end
    # //---- (次の関数ヘッダの先頭)
    if line_text =~ /\s+\/{2}----/
      @step = 0
    end
  end
  #----------------------------------------------------------------------------
  # 
  #----------------------------------------------------------------------------
  def step4_find_opt_end(line_text)
    # //---- (次の関数ヘッダの先頭)
    if line_text.include?("DECOPTEND*/")
      @current_func_doc_decl_text.opt_text = @current_func_doc_opt_script
      @current_func_doc_opt_script = ""
      @step = 0
      return
    end
    # //---- (次の関数ヘッダの先頭)
    if line_text =~ /\s+\/{2}----/
      @step = 0
      return
    end
    
    @current_func_doc_opt_script += line_text
  end
end
