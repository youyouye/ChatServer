package com.muduo.chat;

import io.netty.buffer.ByteBuf;
import java.nio.charset.Charset;


public class DataEvent extends Event {

    public final EventSource source;
    public final int whichClient;
    public final ByteBuf data;

    public DataEvent(EventSource source, int whichClient, ByteBuf data) {
        this.source = source;
        this.whichClient = whichClient;
        this.data = data;
    }

    public String getString() {
        return data.toString(Charset.defaultCharset());
    }
}
