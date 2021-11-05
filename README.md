# tcpjack kernel module


Linuxカーネルモジュールを利用する機会があったため、基本的な動作原理と作成方法をおさえる。

チュートリアル: [The Linux Kernel Module Programming Guide](https://tldp.org/LDP/lkmpg/2.6/html/lkmpg.html#AEN121)


## 仕様

- ローカルホスト(127.0.0.1)宛の全TCPパケットについて、TCP宛先ポートを9999に変更する
- ※ローカルホストの通信が正しく動作しなくなるため、tcpjackカーネルモジュールを動作させるさいには注意




## メモ

- VSCodeのIntelliSense Configurationsで、Include Pathを以下のように修正
```
"includePath": [
    "${workspaceFolder}/**",
    "/usr/src/linux-aws-5.11-headers-5.11.0-1020/include" // 追加
],
```

## 参考
- https://blog.mono0x.net/2015/02/24/netfilter/