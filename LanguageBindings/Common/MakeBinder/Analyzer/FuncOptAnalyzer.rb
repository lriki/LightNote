# coding: utf-8
#==============================================================================
# ■ FuncOptAnalyzer
#------------------------------------------------------------------------------
#   
#==============================================================================

# eval からのアクセス用
$opt = nil

STATIC_METHOD = 0
CLASS_INSTANCE = 1
STRUCT_INSTANCE = 2

# C# 用オプション
class CSOpt
  attr_accessor :enable
  attr_accessor :constructor
  attr_accessor :destructor
  attr_accessor :property         # [プロパティ名, アクセサ名(get/set)]
  attr_accessor :host_field_type  # C# での型名。フィールド名の他、プロパティの型にも使用する。
  attr_accessor :host_field       # 値を保持するフィールド名。指定すると、host_field_type の型でフィールドが作られる
  #attr_accessor :instance_method  # [未使用…]先頭の lnHandle をクラスインスタンスのハンドルとする。デフォルト true
  attr_accessor :method_kind      # STATIC_METHOD or CLASS_INSTANCE or STRUCT_INSTANCE
  attr_accessor :method_name      # C#のメソッドを別名にする
  attr_accessor :return_out_arg   # (-1) x番目の引数を戻り値として返す。0スタート。構造体限定
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
  # host_field_type ユーティリティ
  def get_host_field_type(return_type)
    if @host_field_type != nil
      return @host_field_type
    else
      return convert_type_to_cs(return_type)
    end
  end
end

# HSP用オプション
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
  attr_accessor :type_override  # [共通opt] 仮引数名の型をオーバーライド ["仮引数名", "新しい型名"]
  attr_accessor :cs     # CSOpt
  attr_accessor :hsp    # HSPOpt
  #----------------------------------------------------------------------------
  # 初期化
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








