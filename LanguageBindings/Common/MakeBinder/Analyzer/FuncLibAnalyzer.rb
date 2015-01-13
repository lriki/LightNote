# coding: utf-8
#==============================================================================
# ■ FuncLibAnalyzer
#------------------------------------------------------------------------------
#   FuncLibAnalyzer ルート
#==============================================================================
require_relative './../Config.rb'
require_relative './FuncHeaderFinder.rb'
require_relative './EnumAnalyzer.rb'
require_relative './FuncHeaderAnalyzer.rb'
require_relative './FuncDeclAnalyzer.rb'
require_relative './FuncOptAnalyzer.rb'

class FuncInfo
  attr_accessor :func_name  # 関数名
  attr_accessor :func_doc   # FuncHeaderAnalyzer
  attr_accessor :func_decl  # FuncDeclAnalyzer
  attr_accessor :func_opt   # FuncOptAnalyzer
  def initialize(doc, decl, opt)
    @func_name = decl.func_name
    @func_doc = doc
    @func_decl = decl
    @func_opt = opt
  end
  # オプション引数を解析し、結果を返す (DocOptionInfo の取得)
  def get_doc_option_info(lang_name)
    return DocOptionInfo.new(self, lang_name)
  end
  # デフォルト引数を検索し、値を返す (引数が無い or 値が無い場合は nil)
  def find_default_arg_value(arg_name)
    arg = @func_decl.arg_list.find{|arg| arg.name == arg_name }
    return arg.default if arg != nil
    return nil
  end
end

# 関数コメントオプション引数の解析結果
class DocOptionInfo
  attr_accessor :enabled    # 有効な関数か (bool値。"disable" による)
  attr_accessor :func_name  # 関数名 (別名がつけられていればそれを指す)
  attr_accessor :overridden # オーバーライドされるか (bool値。これによる出力関数名の変更は言語ごとに異なるので、上側で処理する)
  
  def initialize(func_info, lang_name)
    @enabled = true
    @func_name = func_info.func_name
    @overridden = false
    option_args = func_info.func_doc.option_map[lang_name]
    for opt in option_args
      case opt.name
        when "disable"  # HSP では公開しない
          @enabled = false
        when "name"     # 別名を付ける
          @func_name = opt.value
        when "override" # フッタでオーバーライドする
          @overridden = true
      end
    end
  end
end

class FuncLibAnalyzer
  #----------------------------------------------------------------------------
  # 
  #----------------------------------------------------------------------------
  attr_accessor :func_info_list  # FuncInfo の配列
  #----------------------------------------------------------------------------
  #
  #----------------------------------------------------------------------------
  def initialize
    @func_info_list = []
  end
  #----------------------------------------------------------------------------
  #
  #----------------------------------------------------------------------------
  def analyze(target_header_files)
    # 全ファイル中の、解析が必要な部分を取りだす
    func_header_finder = FuncHeaderFinder.new
    for file_path in $target_files
      func_header_finder.analyze($lnote_root + file_path)
    end
    
    # 取りだした部分を解析
    for func_doc in func_header_finder.func_text_list
      func_doc_analyzer = FuncHeaderAnalyzer.new
      func_doc_analyzer.analyze(func_doc.doc_text, func_doc.doc_opt_text)
      func_decl_analyzer = FuncDeclAnalyzer.new
      func_decl_analyzer.analyze(func_doc.decl_text)
      func_opt_analyzer = FuncOptAnalyzer.new
      func_opt_analyzer.analyze(func_doc.opt_text)
      
      func_info = FuncInfo.new(func_doc_analyzer, func_decl_analyzer, func_opt_analyzer)
      @func_info_list.push(func_info)
    end
  end
  #----------------------------------------------------------------------------
  # 関数名から FuncInfo (各関数のヘッダ、定義) を検索する
  #----------------------------------------------------------------------------
  def find_func_info(func_name)
    return @func_info_list.find{|f| f.func_name == func_name }
  end
  #----------------------------------------------------------------------------
  # モジュール名に属する FuncInfo の配列を取得する
  #----------------------------------------------------------------------------
  def find_func_info_list_by_module(module_name)
    return @func_info_list.select{|f| f.func_decl.module_name == module_name }
  end
end
