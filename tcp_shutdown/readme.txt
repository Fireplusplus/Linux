gcc tcp_win.c -o tcp.exe -lws2_32

(1)
api:
linux: shutdown(sock, SHUT_WR);
win: shutdown(sock, SD_SEND)
是否可以看出操作：可以
	shutdown关闭写会触发FIN：如果close了，继续发数据会RST；如果关闭写，则会继续回ACK；所以可以看后续数据的响应。
(2)
api：
linux: shutdown(sock, SHUT_RD)
win: shutdown(sock, SD_RECEIVE)

是否可以看出操作：不可以
linux: A会给B回ACK，数据流看起来跟未执行shutdown一样
win: A会给B回RST，A继续send也会失败