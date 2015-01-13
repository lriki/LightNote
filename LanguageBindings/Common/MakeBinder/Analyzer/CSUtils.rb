# coding: utf-8
#==============================================================================
# �� CSUtils
#------------------------------------------------------------------------------
#   C# ��͗p���[�e�B���e�B
#==============================================================================




$type_convert_table = {
  # C++ Type                �ʏ�ϊ�        API�������^�C���q   �������^      �������C���q
  "lnHandle"            =>  ["IntPtr",      "",                 "IntPtr",     ""],
  "lnHandle*"           =>  ["IntPtr",      "out",              "IntPtr",     ""],
  "void"                =>  ["void",        "",                 "void",       ""],
  "void*"               =>  ["IntPtr",      "",                 "IntPtr",     ""],
  "const char*"         =>  ["string",      "",                 "string",     ""],
  "const TCHAR*"        =>  ["string",      "",                 "string",     ""],
  "const lnChar*"       =>  ["string",      "",                 "string",     ""],
  "int"                 =>  ["int",         "",                 "int",        ""],
  "float"               =>  ["float",       "",                 "float",      ""],
  "int*"                =>  ["int",         "out",              "int",        ""],
  "float*"              =>  ["float",       "out",              "float",      ""],
  "lnBool"              =>  ["bool",        "",                 "bool",       ""],
  "lnByte*"             =>  ["byte[]",      "",                 "byte[]",     ""],
  "const lnByte*"       =>  ["byte[]",      "",                 "byte[]",     ""],
  "lnU32"               =>  ["uint",        "",                 "uint",       ""],
  "const lnVector2*"    =>  ["Vector2",     "ref",              "Vector2",    ""],
  "const lnVector3*"    =>  ["Vector3",     "ref",              "Vector3",    ""],
  "const lnVector4*"    =>  ["Vector4",     "ref",              "Vector4",    ""],
  "const lnMatrix*"     =>  ["Matrix",      "ref",              "Matrix",     ""],
  "const lnQuaternion*" =>  ["Quaternion",  "ref",              "Quaternion", ""],
  "const lnRect*"       =>  ["Rect",        "ref",              "Rect",       ""],
  "const lnColor*"      =>  ["Color",       "ref",              "Color",      ""],
  "lnSize*"             =>  ["Size",        "ref",              "Size",       ""],
  "lnVector2*"          =>  ["Vector2",     "ref",              "Vector2",    ""],
  "lnVector3*"          =>  ["Vector3",     "ref",              "Vector3",    ""],
  "lnVector4*"          =>  ["Vector4",     "ref",              "Vector4",    ""],
  "lnMatrix*"           =>  ["Matrix",      "ref",              "Matrix",     ""],
  "lnQuaternion*"       =>  ["Quaternion",  "ref",              "Quaternion", ""],
  "lnRect*"             =>  ["Rect",        "ref",              "Rect",       ""],
  "lnColor*"            =>  ["Color",       "ref",              "Color",      ""],
  "lnExceptionCallback" =>  ["ExceptionCallback", "",           "ExceptionCallback",      ""],
}
=begin

$module_convert_table = {
  # C++ Module              C# �N���X��         # �^���
  "LConfig"             =>  ["Config",          TYPE_STATIC_MODULE]
  "LTexture"            =>  ["Texture",         TYPE_CLASS]
  "LSNode"              =>  ["Node",            TYPE_CLASS]
  "LSSprite"            =>  ["Sprite",          TYPE_CLASS]
  "LSSpritePath"        =>  ["SpritePath",      TYPE_CLASS]
  "LSMesh"              =>  ["Mesh",            TYPE_CLASS]
  "LVector2"            =>  ["Vector2",         TYPE_STRUCT]
  "LVector3"            =>  ["Vector3",         TYPE_STRUCT]
  "LVector4"            =>  ["Vector4",         TYPE_STRUCT]
  "Matrix"              =>  ["Matrix",          TYPE_STRUCT]
  "Quaternion"          =>  ["Quaternion",      TYPE_STRUCT]
  "Rect"                =>  ["Rect",            TYPE_STRUCT]
  "Color"               =>  ["Color",           TYPE_STRUCT]
}
=end

#----------------------------------------------------------------------------
# C++ �^�� �� C# �^�� �ϊ� (�ʏ�B�錾�̖߂�l�Ȃ�)
#----------------------------------------------------------------------------
def convert_type_to_cs_basic(cpp_type)
  t = cpp_type.gsub(/\s+/, " ")  # �A������X�y�[�X��1�ɕϊ�
  e = $type_convert_table[t]
  # �e�[�u���Ɍ�������
  return e[0] if e != nil
  # enum �����B3�����ڂ���I�[�܂łƂ肾�� ("LN" ���폜)
  return cpp_type.slice(2..-1) if $enum_analyzer.find_enum(cpp_type) != nil
  # �������s
  raise "convert_type_to_cs_basic miss. [" + cpp_type + "]\n"
end

#----------------------------------------------------------------------------
# C++ Type �� C# Type �ϊ� (�N���X������)
#----------------------------------------------------------------------------
def convert_type_to_cs_parameter(cpp_type)
  t = cpp_type.gsub(/\s+/, " ")  # �A������X�y�[�X��1�ɕϊ�
  e = $type_convert_table[t]
  # �e�[�u���Ɍ�������
  return e[2] if e != nil
  # enum �����B3�����ڂ���I�[�܂łƂ肾�� ("LN" ���폜)
  return cpp_type.slice(2..-1) if $enum_analyzer.find_enum(cpp_type) != nil
  # �������s
  raise "convert_type_to_cs_parameter miss. [" + cpp_type + "]\n"
end

#----------------------------------------------------------------------------
# C++ �^�� �� C# �������� �ϊ� (API�Ăяo�����̎�����)
#----------------------------------------------------------------------------
def convert_arg_name_to_cs_for_api(func_arg)  # FuncArg
  t = func_arg.type.gsub(/\s+/, " ")  # �A������X�y�[�X��1�ɕϊ�
  e = $type_convert_table[t]
  # �e�[�u���Ɍ�������
  return e[1] + " " + func_arg.name if e != nil
  # enum �����B3�����ڂ���I�[�܂łƂ肾�� ("LN" ���폜)
  return func_arg.name if $enum_analyzer.find_enum(func_arg.type) != nil
  # �������s
  raise "convert_arg_name_to_cs_for_api miss. [" + func_arg.name + "]\n"
end

#----------------------------------------------------------------------------
# C++ �^�� �� C# �������� �ϊ� (extern API��`�p)
#----------------------------------------------------------------------------
def convert_param_type_to_cs_for_api(param_type)
  t = param_type.gsub(/\s+/, " ")  # �A������X�y�[�X��1�ɕϊ�
  e = $type_convert_table[t]
  # �e�[�u���Ɍ�������
  return e[1] + " " + e[2] if e != nil
  # enum �����B3�����ڂ���I�[�܂łƂ肾�� ("LN" ���폜)
  return param_type.slice(2..-1) if $enum_analyzer.find_enum(param_type) != nil
  # �������s
  raise "convert_param_type_to_cs_for_api miss. [" + param_type + "]\n"
end

#----------------------------------------------------------------------------
# C++ �^�� �� C# �������� �ϊ� (API�Ăяo�����̎�����)
#----------------------------------------------------------------------------
def convert_arg_name_to_cs_for_api(func_arg)  # FuncArg
  t = func_arg.type.gsub(/\s+/, " ")  # �A������X�y�[�X��1�ɕϊ�
  e = $type_convert_table[t]
  # �e�[�u���Ɍ�������
  return e[1] + " " + func_arg.name if e != nil
  # enum �����B3�����ڂ���I�[�܂łƂ肾�� ("LN" ���폜)
  return func_arg.name if $enum_analyzer.find_enum(func_arg.type) != nil
  # �������s
  raise "convert_arg_name_to_cs_for_api miss. [" + func_arg.name + "]\n"
end

#----------------------------------------------------------------------------
# C++���W���[�����̌^��ʂ��擾
#----------------------------------------------------------------------------
=begin
def get_cpp_module_type(cpp_module)
  t = param_type.gsub(/\s+/, " ")  # �A������X�y�[�X��1�ɕϊ�
  e = $module_convert_table[t]
  # �e�[�u���Ɍ�������
  return e[1] if e != nil
  # �������s
  raise "get_cpp_module_type miss. [" + param_type + "]\n"
end
=end

#----------------------------------------------------------------------------
# ���W���[���� �� �N���X���̕ϊ�
#----------------------------------------------------------------------------
def convert_module_to_class_name(module_name)
  case module_name
    when "LConfig"
      return "Config"
    when "LTexture"
      return "Texture"
    when "LSNode"
      return "Node"
    when "LSSprite"
      return "Sprite"
    when "LSSpritePath"
      return "SpritePath"
    when "LSMesh"
      return "Mesh"
    when "LShader"
      return "Shader"
  end
  raise "convert_module_to_class_name miss. [" + module_name + "]\n"
end

#----------------------------------------------------------------------------
# �v���~�e�B�u�^���H
#   �E�Q�Ɠn���̊m�F�̎��A�v���~�e�B�u�^�Ȃ� out�A����ȊO�Ȃ� ref ��t����
#----------------------------------------------------------------------------
def value_cs_primitive_type?(cs_type)
  case cs_type
    when "int"
      return true
    when "uint"
      return true
    when "float"
      return true
    when "double"
      return true
    when "string"   # string ���������I�ɂ� primitive�B���� new �Ƃ����Ȃ�
      return true
  end
  # enum ���������I�Ƀv���~�e�B�u
  if $enum_analyzer.find_enum("ln" + cs_type) != nil
    return true
  end
  return false
end

#----------------------------------------------------------------------------
# �l�^(struct)�Ƃ��Ĉ����^���H (�l�^�����ǁAnew ���Ђ悤�Ȃ���)
#----------------------------------------------------------------------------
def cs_struct_type?(cs_type)
  return true if value_cs_primitive_type?(cs_type)
  
  case cs_type
    when "Rect"
      return true
    when "Vector2"
      return true
    when "Vector3"
      return true
    when "Vector4"
      return true
    when "Matrix"
      return true
    when "Quaternion"
      return true
    when "Color"
      return true
  end
  return false
end
































#----------------------------------------------------------------------------
# �C���q (const �� &, *) ���l�����Ȃ� C++ �^ �� C# �^
#----------------------------------------------------------------------------
def convert_type_to_cs_no_modifier(cpp_type)
  return "IntPtr" if cpp_type.include?("lnHandle")
  return "string" if cpp_type.include?("TCHAR")
  return "int" if cpp_type.include?("int")
  return "float "if cpp_type.include?("float")
  return "Vector2" if cpp_type.include?("lnVector2")
  return "Vector3" if cpp_type.include?("lnVector3")
  return "Vector4" if cpp_type.include?("lnVector4")
  return "Matrix" if cpp_type.include?("lnMatrix")
  return "Quaternion" if cpp_type.include?("lnQuaternion")
  return "Rect" if cpp_type.include?("lnRect")
  return "Color" if cpp_type.include?("lnColor")
  
  # enum �ϊ�
  if $enum_analyzer.find_enum(cpp_type) != nil
    return cpp_type.slice(2..-1)  # 3�����ڂ���I�[�܂łƂ肾�� ("LN" ���폜)
  end
  
  raise "failed convert_type_to_cs_no_modifier. [" + cpp_type + "]"
end

#----------------------------------------------------------------------------
# C++ �̌^�� C# �ɕϊ�����
#----------------------------------------------------------------------------
def _convert_type_to_cs(cpp_type)
  # void �|�C���^
  if cpp_type.include?("void*")
    if cpp_type.include?("const")   # const void*
      return "byte[]"
    else
      return "IntPtr"
    end
  end
  # ���͕�����^
  if cpp_type.include?("const") and cpp_type.include?("TCHAR*")
    return "string"
  end
  # ���͕�����^
  if cpp_type.include?("const") and cpp_type.include?("char*")
    return "string"
  end
  # lnHandle
  if cpp_type.include?("lnHandle")
    return "IntPtr"
  end
  # lnBool
  if cpp_type.include?("lnBool")
    return "bool"
  end
  # lnU32
  if cpp_type.include?("lnU32")
    return "uint"
  end
  
  
  # int*
  if cpp_type.include?("int*")
    if cpp_type.include?("const")
      return "int"
    else
      return "out int"
    end
  end
  # float*
  if cpp_type.include?("float*")
    if cpp_type.include?("const")
      return "float"
    else
      return "out float"
    end
  end
  # lnVector2*, const lnVector2*
  if cpp_type.include?("lnVector2*")
    return "ref Vector2"
  end
  # lnVector3*, const lnVector3*
  if cpp_type.include?("lnVector3*")
    return "ref Vector3"
  end
  # lnVector4*, const lnVector4*
  if cpp_type.include?("lnVector4*")
    return "ref Vector4"
  end
  # lnMatrix*, const lnMatrix*
  if cpp_type.include?("lnMatrix*")
    return "ref Matrix"
  end
  # lnQuaternion*, const lnQuaternion*
  if cpp_type.include?("lnQuaternion*")
    return "ref Quaternion"
  end

  # lnRect*, const lnRect*
  if cpp_type.include?("lnRect*")
    return "ref Rect"
  end
  # lnColor*, const lnColor*
  if cpp_type.include?("lnColor*")
    return "ref Color"
  end
  
  # ���̂܂�
  return cpp_type
end

#----------------------------------------------------------------------------
# C++ �̌^�� C# �ɕϊ����� (���������X�g�p)
#----------------------------------------------------------------------------
=begin
def convert_type_to_formal_parameter_type(cpp_type)
  return convert_type_to_cs(cpp_type)
  text = ""
  # �|�C���^�^�ł���
  if cpp_type.include?("*")
    text += "ref "
  end
  
  text += convert_type_to_cs(cpp_type)
  text.delete!("*")
  return text
end
=end
=begin
def convert_type_to_cs(type_name)
  type_name = _convert_type_to_cs(type_name)
  if $enum_analyzer.find_enum(type_name) != nil
    return type_name.slice(2..-1)  # 3�����ڂ���I�[�܂łƂ肾�� ("LN" ���폜)
  end
  return type_name;
end
=end
