# coding: utf-8
#==============================================================================
# �� FuncLibAnalyzer
#------------------------------------------------------------------------------
#   FuncLibAnalyzer ���[�g
#==============================================================================
require_relative './../Config.rb'
require_relative './FuncHeaderFinder.rb'
require_relative './EnumAnalyzer.rb'
require_relative './FuncHeaderAnalyzer.rb'
require_relative './FuncDeclAnalyzer.rb'
require_relative './FuncOptAnalyzer.rb'

class FuncInfo
  attr_accessor :func_name  # �֐���
  attr_accessor :func_doc   # FuncHeaderAnalyzer
  attr_accessor :func_decl  # FuncDeclAnalyzer
  attr_accessor :func_opt   # FuncOptAnalyzer
  def initialize(doc, decl, opt)
    @func_name = decl.func_name
    @func_doc = doc
    @func_decl = decl
    @func_opt = opt
  end
  # �I�v�V������������͂��A���ʂ�Ԃ� (DocOptionInfo �̎擾)
  def get_doc_option_info(lang_name)
    return DocOptionInfo.new(self, lang_name)
  end
  # �f�t�H���g�������������A�l��Ԃ� (���������� or �l�������ꍇ�� nil)
  def find_default_arg_value(arg_name)
    arg = @func_decl.arg_list.find{|arg| arg.name == arg_name }
    return arg.default if arg != nil
    return nil
  end
end

# �֐��R�����g�I�v�V���������̉�͌���
class DocOptionInfo
  attr_accessor :enabled    # �L���Ȋ֐��� (bool�l�B"disable" �ɂ��)
  attr_accessor :func_name  # �֐��� (�ʖ��������Ă���΂�����w��)
  attr_accessor :overridden # �I�[�o�[���C�h����邩 (bool�l�B����ɂ��o�͊֐����̕ύX�͌��ꂲ�ƂɈقȂ�̂ŁA�㑤�ŏ�������)
  
  def initialize(func_info, lang_name)
    @enabled = true
    @func_name = func_info.func_name
    @overridden = false
    option_args = func_info.func_doc.option_map[lang_name]
    for opt in option_args
      case opt.name
        when "disable"  # HSP �ł͌��J���Ȃ�
          @enabled = false
        when "name"     # �ʖ���t����
          @func_name = opt.value
        when "override" # �t�b�^�ŃI�[�o�[���C�h����
          @overridden = true
      end
    end
  end
end

class FuncLibAnalyzer
  #----------------------------------------------------------------------------
  # 
  #----------------------------------------------------------------------------
  attr_accessor :func_info_list  # FuncInfo �̔z��
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
    # �S�t�@�C�����́A��͂��K�v�ȕ�������肾��
    func_header_finder = FuncHeaderFinder.new
    for file_path in $target_files
      func_header_finder.analyze($lnote_root + file_path)
    end
    
    # ��肾�������������
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
  # �֐������� FuncInfo (�e�֐��̃w�b�_�A��`) ����������
  #----------------------------------------------------------------------------
  def find_func_info(func_name)
    return @func_info_list.find{|f| f.func_name == func_name }
  end
  #----------------------------------------------------------------------------
  # ���W���[�����ɑ����� FuncInfo �̔z����擾����
  #----------------------------------------------------------------------------
  def find_func_info_list_by_module(module_name)
    return @func_info_list.select{|f| f.func_decl.module_name == module_name }
  end
end
