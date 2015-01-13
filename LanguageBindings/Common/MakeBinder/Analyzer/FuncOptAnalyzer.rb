# coding: utf-8
#==============================================================================
# �� FuncOptAnalyzer
#------------------------------------------------------------------------------
#   
#==============================================================================

# eval ����̃A�N�Z�X�p
$opt = nil

STATIC_METHOD = 0
CLASS_INSTANCE = 1
STRUCT_INSTANCE = 2

# C# �p�I�v�V����
class CSOpt
  attr_accessor :enable
  attr_accessor :constructor
  attr_accessor :destructor
  attr_accessor :property         # [�v���p�e�B��, �A�N�Z�T��(get/set)]
  attr_accessor :host_field_type  # C# �ł̌^���B�t�B�[���h���̑��A�v���p�e�B�̌^�ɂ��g�p����B
  attr_accessor :host_field       # �l��ێ�����t�B�[���h���B�w�肷��ƁAhost_field_type �̌^�Ńt�B�[���h�������
  #attr_accessor :instance_method  # [���g�p�c]�擪�� lnHandle ���N���X�C���X�^���X�̃n���h���Ƃ���B�f�t�H���g true
  attr_accessor :method_kind      # STATIC_METHOD or CLASS_INSTANCE or STRUCT_INSTANCE
  attr_accessor :method_name      # C#�̃��\�b�h��ʖ��ɂ���
  attr_accessor :return_out_arg   # (-1) x�Ԗڂ̈�����߂�l�Ƃ��ĕԂ��B0�X�^�[�g�B�\���̌���
  def initialize
    @enable = true
    @constructor = false
    @destructor = false
    @property = []
    @host_field_type = nil
    @host_field = nil
    #@instance_method = true
    @method_kind = CLASS_INSTANCE
    @method_name = nil
    @return_out_arg = -1
  end
  # host_field_type ���[�e�B���e�B
  def get_host_field_type(return_type)
    if @host_field_type != nil
      return @host_field_type
    else
      return convert_type_to_cs(return_type)
    end
  end
end

# HSP�p�I�v�V����
class HSPOpt
  attr_accessor :enable
  attr_accessor :override
  def initialize
    @enable = true
    @override = false
  end
end

class FuncOptAnalyzer
  #----------------------------------------------------------------------------
  # 
  #----------------------------------------------------------------------------
  attr_accessor :type_override  # [����opt] ���������̌^���I�[�o�[���C�h ["��������", "�V�����^��"]
  attr_accessor :cs     # CSOpt
  attr_accessor :hsp    # HSPOpt
  #----------------------------------------------------------------------------
  # ������
  #----------------------------------------------------------------------------
  def initialize
    @type_override = nil
    @cs = CSOpt.new
    @hsp = HSPOpt.new
  end
  #----------------------------------------------------------------------------
  # 
  #----------------------------------------------------------------------------
  def analyze(text)
    $opt = self
    if text != nil
      eval text
    end
    $opt = nil
  end
end








