=begin
��Ƃ� Cygwin �ōs���B

�܂��͍�ƃf�B���N�g���Ɉȉ��̃t�@�C�����R�s�[
extconf.rb
lnote_ruby.cpp
lnote_ruby.h

$ /cygdrive/d/Proj/lightnote/LanguageBindings/Ruby
$ extconf.rb

=end

#/cygdrive/d/Proj/lightnote/LanguageBindings/Ruby
#cd /d D:\Proj\lightnote\LanguageBindings\Ruby
#extconf.rb

#ruby extconf.rb --with-opt-include=/cygdrive/d/Proj/lightnote/LightNote/Source
#make
#make install
#ruby Test.rb

require 'mkmf'

have_library("LNote")

#have_library("stdc++")
create_makefile("lnote")
