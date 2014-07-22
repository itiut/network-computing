Network Computing 2014
====

2014年度夏学期におけるネットワークコンピューティングの演習課題です。

[講義ページ](http://lecture.nc.u-tokyo.ac.jp/)

課題1 `daytime`クライアントの作成
----
### プログラムリスト
* [daytime/daytime.h](/daytime/daytime.h)
* [daytime/daytime.c](/daytime/daytime.c)

### 取り組んだ追加課題
* 引数による宛先アドレス指定
* 適切なエラー処理
* IPv6対応 (オプションによる指定)

課題2 HTTPサーバの作成
----
### プログラムリスト
* [httpd/httpd_fork.c](/httpd/httpd_fork.c)
  * `fork`版
* [httpd/httpd_select.c](/httpd/httpd_select.c)
  * `select`版
* [httpd/httpd_thread.c](/httpd/httpd_thread.c)
  * `pthread`版
* [httpd/server.c](/httpd/server.c)
  * HTTPリクエスト・レスポンスを処理するプログラム

### 取り組んだ課題・機能
* `fork`, `select`, `pthread`のそれぞれで並行サーバを実装
* HTTPリクエストヘッダの1行目をパース
* `GET`と`HEAD`メソッドに対応
* それ以外のメソッドでは`501 Not Implemented`レスポンスを返す

課題3 チャットサーバ・クライアントの作成
----
### プログラムリスト
* [chat/chatd.c](/chat/chatd.c)
  * サーバ
* [chat/chat.c](/chat/chat.c)
  * クライアント

### 機能
* `epoll`を用いた多重化 (サーバ)
* 複数人同時接続
* 1対多へのメッセージ配信
* メンバーの動的な増減
* IPv4 / IPv6 デュアルスタック
* ユーザー毎に表示色を変更 (クライアント)
