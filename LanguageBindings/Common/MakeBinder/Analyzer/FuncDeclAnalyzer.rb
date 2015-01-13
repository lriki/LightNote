# coding: utf-8
#==============================================================================
# �� FuncDeclAnalyzer
#------------------------------------------------------------------------------
#   FuncHeaderFinder �Œ��o�������e�𕪉�(�֐��錾1�s��)
#==============================================================================
require_relative './../Config.rb'

class FuncArg
  attr_accessor :name         # ������
  attr_accessor :default      # �f�t�H���g����
  attr_accessor :type         # �^
  attr_accessor :attr_ref     # �l�Q�ƃ^�C�v (const �L��|�C���^�^�ł��邩�BC#��ref)
  attr_accessor :attr_out     # �o�̓^�C�v (const �����|�C���^�^�ł��邩�BC#��out)
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
  attr_accessor :func_name    # �֐���
  attr_accessor :return_type  # �߂�l�̌^��
  attr_accessor :arg_list     # �������X�g (FuncArg �̔z��)
  attr_accessor :module_name  # ���W���[���� (�֐��� _ �̑O�̕���)
  attr_accessor :method_name  # ���\�b�h�� (�֐��� _ �̌��̕���)
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
  # FuncArg �̌���
  #----------------------------------------------------------------------------
  def find_func_arg(arg_name)
    return @arg_list.find{|arg| arg.name == arg_name }
  end
  #----------------------------------------------------------------------------
  # 
  #----------------------------------------------------------------------------
  def analyze(decl_line)
    unless decl_line.include?(");")
      # �֐��I�[�� ); ��������Ȃ����� (�ԂɃX�y�[�X������Ȃ�)
      raise "); termination of the function does not exist.\n" + decl_line
    end
    if decl_line.include?("=")
      # ���ʂ̃f�t�H���g�������̓G���[
      raise "use LNOTE_DEFAULT_ARG the default argument.\n" + decl_line
    end
    
    # (); �Ƃ��̑O�Ƃɕ�����   "void Func" "(int x);"
    decl_line =~ /\(.*\);/
    arg_str = $&
    
    # �֐����E�^�� ('('���O�̃g�[�N�����֐����B����ȑO�͂��ׂČ^���ƂȂ�)
    $` =~ /\S+$/
    @func_name = $&
    @return_type = $`.strip
    
    # ���� - �O��̊��ʂ��폜���A����
    #     LNOTE_DEFAULT_ARG(LN_ROTATIONORDER_XYZ) ���c���B
    #arg_str.delete!("()")
    arg_str.slice!("(")
    arg_str.slice!(");")
    args = arg_str.split(",")
    for a in args
      arg_decl = FuncArg.new
      a.strip!
      
=begin
      # '=' �ŕ��� (�f�t�H���g���������邩�H)
      pair = a.split("=")
      arg_decl.name = pair[0]
      if pair.size != 1 # �f�t�H���g����������
        arg_decl.default = pair[1]
        arg_decl.default.strip!
      end
=end
      # 'LNOTE_DEFAULT_ARG' �̑O��ŕ��� (�f�t�H���g���������邩�H)
      pair = a.split("LNOTE_DEFAULT_ARG")
      arg_decl.name = pair[0]
      if pair.size != 1 # �f�t�H���g����������
        arg_decl.default = pair[1].delete("()");  # �}�N���O��� ( ) ���폜
        arg_decl.default.strip!
      end
      
      # �^���Ɩ��O�ɕ�����
      arg_decl.name.strip!
      arg_decl.name =~ /\S+$/
      arg_decl.name = $&
      arg_decl.type = $`.strip
      arg_decl.post_check
      @arg_list.push(arg_decl)
    end
    
    # �֐������烂�W���[�������擾 (_ �ŕ���)
    pair = @func_name.split("_")
    @module_name = pair[0]
    @method_name = pair[1]
  end
end

