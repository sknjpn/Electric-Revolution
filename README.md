# Electric-Revolution
# [ダウンロード先](https://github.com/sknjpn/Electric-Revolution/wiki/%E3%83%80%E3%82%A6%E3%83%B3%E3%83%AD%E3%83%BC%E3%83%89)

## ビルド環境(Tested)

- Windows7 Professional 64bit
- Visual Studio Community 2017
- OpenSiv3D ver0.1.7
- Git fow Windows or SourceTree

## セットアップ手順

### OpenSiv3Dインストール

- [ここ](https://github.com/Siv3D/OpenSiv3D/wiki/OpenSiv3D-SDK-for-Windows-Desktop)からダウンロード
- インストール&PC再起動

### ソースコードのダウンロード

- 任意のフォルダに移動して以下コマンド
  ~~~
  git clone https://github.com/SKN-JP/Electric-Revolution.git
  ~~~

### Visual Studio Community 2017インストール

- [公式サイト](https://www.visualstudio.com/ja/downloads/?rr=https%3A%2F%2Fwww.microsoft.com%2Fja-jp%2Fdev%2Fproducts%2Fcommunity.aspx)からダウンロード
- インストールパッケージは以下を選択
  - 「ワークロード」タブ
    - C++によるデスクトップ開発
  - 「個別のコンポーネント」タブ
    - NuGetパッケージマネージャ
    - Windows 8.1 SDK
    - Windows 10 SDK(10.0.15063.0) for Desktop C++[x86 and x64]
    - UWP用 Windows 10 SDK(10.0.15063.0): C#、VB、JS
    - UWP用 Windows 10 SDK(10.0.15063.0): C++
- インストール完了したら起動

### プロジェクト作成

- 「ファイル」->「開く」->「プロジェクト/ソリューション」を選択
- git cloneしたフォルダ内の「Electric-Revolution.sln」を選択

### Lua 5.30のインストール

- Visual Studio画面の「ツール」->「NuGetパッケージマネージャ」->「ソリューションのNuGetパッケージの管理」を選択
- 検索欄に「lua」を入力、luaを選択する。(lua.binariesではない)
- インストール先のプロジェクトを選択し、バージョンは5.3系の最新を選択しインストールする。

### ビルド

- Ctrl+Shift+B
