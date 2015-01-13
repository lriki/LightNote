# coding: utf-8
#==============================================================================
# ■ 共通設定
#==============================================================================

# ライブラリソースのルート
$lnote_root = File.expand_path(File.dirname(__FILE__) + "/../../../LightNote/Source") + "/"

# ライブラリバージョン
$lnote_version = "1.10"

# 対象ファイル
$target_files = [
  "C_API/LFBase.h",
  "C_API/LFMath.h",
  "C_API/LFFile.h",
  "C_API/LFInput.h",
  "C_API/LFAudio.h",
  "C_API/LFGraphics.h",
  "C_API/LFScene.h",
  "C_API/LFFramework.h"
]

# enum 検索用ファイル
#   関数ライブラリ用の enum、ということで混乱しないよう、
#   必要な enum はすべて LFTypedef.h に集めておく。
#   これ以外の .h は基本的に追加しないこと。
$enum_files = [
  "C_API/LFTypedef.h"
]

# 対象ファイルのエンコーディング
$target_files_encoding = "utf-8"

# 解析前に置換する文字列 (コメント内も含め、ソース全体が対象)
$prepro_replace = [
  ["LNOTEAPI", ""]
]

# 言語別オプション引数の言語名
$option_arg_lang_names = [
  "cs", "hsp"
]

#==============================================================================
#
#==============================================================================
