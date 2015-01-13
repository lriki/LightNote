# coding: utf-8
#==============================================================================
# �� CSXMLMain
#------------------------------------------------------------------------------
#   ���̃t�@�C���͎��̃t�@�C��������s�����B
#   Source/DotNet/LightNote.DotNet/LightNote.DotNet/ConvertXMLDocSource.bat
#
#   �u//#CLASS_BODY.���W���[�����v�̂�����ɁA
#   ��͂��Đ������ꂽ���\�b�h��`���W�J�����B
#==============================================================================
require_relative './Analyzer/FuncLibAnalyzer.rb'
require_relative './Analyzer/CSUtils.rb'

$func_xmldoc_summary_template = "        /// <summary>\n        /// SUMMARY\n        /// </summary>"
$func_xmldoc_param_template = "        /// <param name=\"PARAM_NAME\">DETAIL</param>"
$func_xmldoc_return_template = "        /// <returns>DETAIL</returns>"

$func_dispose_template = "public void Dispose()\n{\n    if (_handle != IntPtr.Zero) {\n        API.API_NAME(_handle);\n        _handle = IntPtr.Zero;\n    }\n    GC.SuppressFinalize(this);\n}\n"

$handle_wrapper_base_class = "ReferenceObjectWrapper"

#==============================================================================
# �� �ݒ肱���܂�
#==============================================================================

class PropertyInfo
  attr_accessor :type          # C++ �^��
  attr_accessor :get_func_info # get ��`��� (FuncInfo)
  attr_accessor :set_func_info # set ��`��� (FuncInfo)
  def initialize
    @type = nil
    @get_func_info = nil
    @set_func_info = nil
  end
end

#==============================================================================
#
#==============================================================================

$this_field_name = "_handle";
INSTANCE_HANDLE_TYPE = "lnHandle"

class PropertyAnalyzer
  #----------------------------------------------------------------------------
  # 
  #----------------------------------------------------------------------------
  attr_accessor :class_prop_hash# �v���p�e�B���X�g {�v���p�e�B��, PropertyInfo}
  #----------------------------------------------------------------------------
  # 
  #----------------------------------------------------------------------------
  def initialize
    @class_prop_hash = {}
  end
  #----------------------------------------------------------------------------
  # 
  #----------------------------------------------------------------------------
  def clear
    @class_prop_hash.clear
  end
  #----------------------------------------------------------------------------
  # 
  #----------------------------------------------------------------------------
  def try_push(func_info)
    unless func_info.func_opt.cs.property.empty?
      # �G���[�`�F�b�N�B�v���p�e�B�ɂł���̂́A�������� this ������ handle �ŁA�S��������2�ȉ��̃��m�Ɍ���
      raise "invalid property - this handle.\n" + func_info.func_name if func_info.func_decl.arg_list[0].type != "lnHandle"
      raise "invalid property - args count.\n" + func_info.func_name if func_info.func_decl.arg_list.size > 2
    
      # �V�����쐬
      unless @class_prop_hash.has_key?(func_info.func_opt.cs.property[0])
        # set �͐�ɓo�^�ł��Ȃ� (value.type �ւ̊i�[���@���C������K�v������)
        #raise "can not be registered before the set. ..." + func_info.func_name + "\n" if func_info.func_opt.cs.property[1] == "set"
      
        info = PropertyInfo.new
        
        type = func_info.func_decl.return_type
        if type == "void" # �߂�l�� void �ł���΂ӂ��߂̈����̌^���g��
          type = func_info.func_decl.arg_list[1].type
        end
        
        info.type = type # �����ł͂܂� C++ �̌^�̂܂�   #func_info.func_opt.cs.get_host_field_type(type)
        @class_prop_hash[func_info.func_opt.cs.property[0]] = info
      end
      
      # get set �U�蕪��
      info = @class_prop_hash[func_info.func_opt.cs.property[0]]
      
      if func_info.func_opt.cs.property[1] == "get"
        info.get_func_info = func_info
      elsif func_info.func_opt.cs.property[1] == "set"
        info.set_func_info = func_info
      end
      
      # ���̒�`�̓X�L�b�v
      return true
    end
    return false
  end  
  #----------------------------------------------------------------------------
  # 
  #----------------------------------------------------------------------------
  def dump
    output_text = ""
  
    @class_prop_hash.each{|key, value|
    
      host_field_type = nil
      host_field_type = value.get_func_info.func_opt.cs.host_field_type if value.get_func_info != nil and value.get_func_info.func_opt.cs.host_field_type != nil
      host_field_type = value.set_func_info.func_opt.cs.host_field_type if value.set_func_info != nil and value.set_func_info.func_opt.cs.host_field_type != nil
      
      # �߂�l����������Ă���ꍇ�͂�����g�p����
      if host_field_type != nil
        output_text += "public " + host_field_type + " " + key + " {\n"
      else
        output_text += "public " + convert_type_to_cs_no_modifier(value.type) + " " + key + " {\n"
      end
      
      cs_no_modifier_type = convert_type_to_cs_no_modifier(value.type)
      
      #-------------------------------------------
      # getter
      if value.get_func_info != nil
        output_text += "    get {\n"
        
        # �l�^ (�v���~�e�B�u�^) �̏ꍇ
        if value_cs_primitive_type?(cs_no_modifier_type)
          text  = "return API.API_NAME(THIS_FIELD_NAME);\n"
          text.gsub!("API_NAME", value.get_func_info.func_name)
          text.gsub!("THIS_FIELD_NAME", $this_field_name)
          
        # �l�^ (struct) �̏ꍇ
        elsif cs_struct_type?(cs_no_modifier_type)
          text  = "VALUE_TYPE v = new VALUE_TYPE();\n"
          text += "API.API_NAME(THIS_FIELD_NAME, ref v);\n"
          text += "return v;\n"
          text.gsub!("VALUE_TYPE", convert_type_to_cs_no_modifier(value.type))
          text.gsub!("API_NAME", value.get_func_info.func_name)
          text.gsub!("THIS_FIELD_NAME", $this_field_name)
        
        # �Q�ƌ^�̏ꍇ
        else
          opt = value.get_func_info.func_opt.cs
          raise "none opt.host_field. [" + value.get_func_info.func_name + "]\n" if opt.host_field == nil
          
          # get �e���v���[�g
          text  = "if (HOLD_FIELD_NAME == null) {\n"
          text += "    HOLD_FIELD_NAME = new WRAPPER_CLASS(API.API_NAME(THIS_FIELD_NAME));\n"
          text += "}\n"
          text += "return HOLD_FIELD_NAME;\n"
          text.gsub!("HOLD_FIELD_NAME", opt.host_field)
          text.gsub!("WRAPPER_CLASS", opt.host_field_type)
          text.gsub!("API_NAME", value.get_func_info.func_name)
          text.gsub!("THIS_FIELD_NAME", $this_field_name)
        end
      
        output_text += text + "    }\n"
      end
      
      #-------------------------------------------
      # setter
      if value.set_func_info != nil
        output_text += "set {\n"
        
        # �l�^ (�v���~�e�B�u�^) �̏ꍇ
        if value_cs_primitive_type?(cs_no_modifier_type)
          text  = "API.API_NAME(THIS_FIELD_NAME, value);\n"
          text.gsub!("API_NAME", value.set_func_info.func_name)
          text.gsub!("THIS_FIELD_NAME", $this_field_name)
          
        # �l�^�̏ꍇ
        elsif cs_struct_type?(cs_no_modifier_type)
          # set �e���v���[�g
          text = "API.API_NAME(THIS_FIELD_NAME, ref value);\n"
          text.gsub!("API_NAME", value.set_func_info.func_name)
          text.gsub!("THIS_FIELD_NAME", $this_field_name)
          
        # �Q�ƌ^�̏ꍇ
        else
          # set �e���v���[�g
          text = "API.API_NAME(THIS_FIELD_NAME, value.THIS_FIELD_NAME);\n"
          text.gsub!("API_NAME", value.set_func_info.func_name)
          text.gsub!("THIS_FIELD_NAME", $this_field_name)
        end
      
        output_text += text + "}\n"
      end
      
      output_text += "}\n"
    }
    
    return output_text
  end
end



#==============================================================================
#
#==============================================================================

# C++ �w�b�_���
analyzer = FuncLibAnalyzer.new
analyzer.analyze($target_files)

# enum ���
$enum_analyzer = EnumAnalyzer.new
for file_path in $enum_files
  $enum_analyzer.analyze($lnote_root + file_path)
end

# �v���p�e�B���
$property_analyzer = PropertyAnalyzer.new

# C# �\�[�X�� 1 �s���ǂ�ł���
$output = ""
$class_body_output = ""
file = File.open(ARGV[0], "r:utf-8")
while line_text = file.gets do
  if line_text.include?("//#")
    # �󔒍폜 > "//" #�폜 > "." �ŕ���
    idents = line_text.strip.delete("//#").split(".")
    
    handle_wrapper_class = idents.include?($handle_wrapper_base_class)
    
    # CLASS_BODY �ł���΃��\�b�h��`��W�J����
    if idents[0] == "CLASS_BODY"
      $class_body_output = ""
      $property_analyzer.clear
      
      # �I�u�W�F�N�g�n���h�� (�Ƃ肠���������ɒu���Ă���)
      #$class_body_output = "private IntPtr _handle;\n\n"
      
      # �t�B�[���h���X�g {�t�B�[���h��, C++�^��}
      class_field_hash = {}
      
      
      # �w�胂�W���[�����̊֐��ꗗ
      func_list = analyzer.find_func_info_list_by_module(idents[1])
      for func_info in func_list
      
        next unless func_info.func_opt.cs.enable
        
         # �t�B�[���h�ɕێ�����l��Ԃ��֐��̏ꍇ
        if func_info.func_opt.cs.host_field != nil
          # �ǉ�����t�B�[���h�Ƃ���
          class_field_hash[func_info.func_opt.cs.host_field] = func_info.func_opt.cs.get_host_field_type(func_info.func_decl.return_type)
        end
        
        #------------------------------------------------------
        # �v���p�e�B��� (�o�^�ł����炱�̊֐��̓X�L�b�v)
        next if $property_analyzer.try_push(func_info)
        
        #------------------------------------------------------
        # ���\�b�h��
        # �R���X�g���N�^�ł���
        if func_info.func_opt.cs.constructor
          # �A�N�Z�X���x��
          $class_body_output += "public "
          
          # ���W���[�������N���X����
          $class_body_output += convert_module_to_class_name(func_info.func_decl.module_name)
          
        # �f�X�g���N�^�ł���
        elsif func_info.func_opt.cs.destructor
          $class_body_output += "~" + convert_module_to_class_name(func_info.func_decl.module_name)
          
        # ���ʂ̃��\�b�h�ł���
        else
          # �A�N�Z�X���x��
          $class_body_output += "public "
          
          # static �L��
          $class_body_output += "static " if func_info.func_opt.cs.method_kind == STATIC_METHOD
          
          # �߂�l
          if func_info.func_opt.cs.return_out_arg > -1
            type = func_info.func_decl.arg_list[func_info.func_opt.cs.return_out_arg].type
            $class_body_output += convert_type_to_cs_basic(type) + " "
          else
            $class_body_output += convert_type_to_cs(func_info.func_decl.return_type) + " " # Type C++ > C#
          end
          
          # �֐���
          if func_info.func_opt.cs.method_name != nil
            $class_body_output += func_info.func_opt.cs.method_name # ���\�b�h����ʖ���
          else
            $class_body_output += func_info.func_decl.method_name
          end
        end
        
        #------------------------------------------------------
        # ���������X�g
        $class_body_output += "("
        args_text = ""
        
        # �f�X�g���N�^�ȊO
        unless func_info.func_opt.cs.destructor
          for i in 0...func_info.func_decl.arg_list.size
            arg = func_info.func_decl.arg_list[i]
            # �C���X�^���X���\�b�h�̏ꍇ�͐擪�� IntPtr ���X�L�b�v
            next if i == 0 and func_info.func_opt.cs.method_kind == CLASS_INSTANCE and arg.type == INSTANCE_HANDLE_TYPE
            # struct ���\�b�h�̏ꍇ�͂����X�L�b�v
            next if i == 0 and func_info.func_opt.cs.method_kind == STRUCT_INSTANCE
            # �߂�l�Ƃ��ĕԂ����������̏ꍇ�̓X�L�b�v (Vector3.Dot �Ƃ�)
            next if i == func_info.func_opt.cs.return_out_arg
            # �e�L�X�g�����
            args_text += ", " if args_text != ""  # 2�ڈȍ~�̈����Ȃ� , ��ǉ�
            args_text += convert_type_to_cs_parameter(arg.type) + " " # Type C++ > C#
            args_text += arg.name
            args_text += "= " + arg.default if arg.default != nil
          end
        end
        $class_body_output += args_text + ")\n"
        
        #------------------------------------------------------
        # �R���X�g���N�^�̏ꍇ�� base �R�[��
        if func_info.func_opt.cs.constructor
          $class_body_output += "    : base(IntPtr.Zero)\n"
        end
      
        #------------------------------------------------------
        # �f�X�g���N�^�p���ꏈ��
        if func_info.func_opt.cs.destructor
          $class_body_output += "{ Dispose(); }\n"
          $class_body_output += $func_dispose_template.gsub("API_NAME", func_info.func_decl.func_name)
        end
        
        #------------------------------------------------------
        # �֐��{��
        
        # �f�X�g���N�^�ȊO
        unless func_info.func_opt.cs.destructor
          # �֐��{�̊J�n
          $class_body_output += "{\n"
          #$class_body_output += " " * 4
          
          
          # �ꎞ�ϐ��Ɋi�[���Ă����Ԃ�����
          if func_info.func_opt.cs.return_out_arg > -1
            type     = func_info.func_decl.arg_list[func_info.func_opt.cs.return_out_arg].type
            arg_name = func_info.func_decl.arg_list[func_info.func_opt.cs.return_out_arg].name
            $class_body_output += "var " + arg_name + " = new " + convert_type_to_cs_basic(type) + "();\n"
          end
          
          # �߂�l�̌^�͂��邩
          unless func_info.func_decl.return_type.empty?
            # �t�B�[���h�ɖ߂�l���i�[����ꍇ
            if func_info.func_opt.cs.host_field != nil
              $class_body_output += func_info.func_opt.cs.host_field + " = "
            elsif func_info.func_decl.return_type != "void"
              $class_body_output += "return "
            end
          end
          
          # ���\�b�h�R�[��
          args_text = ""
          $class_body_output += "API." + func_info.func_decl.func_name + "(";
          
          # ���������X�g
          #for arg in func_info.func_decl.arg_list
          for i in 0...func_info.func_decl.arg_list.size
            arg = func_info.func_decl.arg_list[i]
            # �C���X�^���X���\�b�h�̏ꍇ�͐擪�� IntPtr �� _handle ��
            if i == 0 and func_info.func_opt.cs.method_kind == CLASS_INSTANCE and arg.type == INSTANCE_HANDLE_TYPE
              args_text += $this_field_name
              next
            end
            # struct ���\�b�h�̏ꍇ�� this ��n��
            if i == 0 and  func_info.func_opt.cs.method_kind == STRUCT_INSTANCE
              args_text += "ref this"
              next
            end
            # 2�ڈȍ~�̈����Ȃ� , ��ǉ�
            args_text += ", " if args_text != ""  
            args_text += convert_arg_name_to_cs_for_api(arg)
          end
          $class_body_output += args_text + ");\n";
          
          # �ꎞ�ϐ��Ɋi�[���Ă����Ԃ�����
          if func_info.func_opt.cs.return_out_arg > -1
            $class_body_output += "return " + func_info.func_decl.arg_list[func_info.func_opt.cs.return_out_arg].name + ";\n"
          end

          # �֐��{�̏I��
          $class_body_output += "}\n\n"
          
        end
      end
      
      #------------------------------------------------------
      # �v���p�e�B��`
      $class_body_output += $property_analyzer.dump
      
      #------------------------------------------------------
      # �t�B�[���h��`
      $class_body_output += "\n"
      class_field_hash.each{|key, value|
        if key != "_handle"   # _handle �͒�`���Ȃ�
          $class_body_output += "internal " + convert_type_to_cs(value) + " " + key + ";\n"
        end
      }
      
      #------------------------------------------------------
      # �K�{���\�b�h
      
      # ��R���X�g���N�^
=begin
      text  = "        internal CLASS_NAME()\n"
      text += "        {\n"
      text += "        }\n"
      text.gsub!("CLASS_NAME", convert_module_to_class_name(idents[1]))
      $class_body_output += text
=end

      # �n���h���R���X�g���N�^
      if handle_wrapper_class
        text  = "        internal CLASS_NAME(IntPtr handle)\n"
        text += "            : base(handle)\n"
        text += "        {\n"
        text += "        }\n"
        text.gsub!("CLASS_NAME", convert_module_to_class_name(idents[1]))
        $class_body_output += text
      end
    
      # �o�͂��Ď��̍s��
      $output += $class_body_output
      next
      
    # ����ȊO��XML�h�L�������g��u������
    else
      # �֐�����
      func_info = analyzer.find_func_info(idents[0])
      if func_info == nil
        p "not found func ... " + idents[0]
      else
        # <summary>
        if idents[1] == "summary"
          $output += $func_xmldoc_summary_template.gsub("SUMMARY",func_info.func_doc.summary) + "\n";
          next
        # <param>
        elsif idents[1] == "param"
          param_info = func_info.func_doc.find_param_info(idents[2])
          if param_info != nil
            t = $func_xmldoc_param_template.gsub("PARAM_NAME", param_info.name);
            t.gsub!("DETAIL", param_info.detail);
            $output += t + "\n"
            next
          else
            p "not found param ... " + idents[2]
          end
        end
      end
    end
  end
  
  $output += line_text
end

# BOM ��t���ďo��
$output = "\xEF\xBB\xBF" + $output
File.open(ARGV[1], "w:utf-8") {|f| f.write $output}

p "[Completed.]"

