use im;
create table if not exists OffineMsg
(
uuid int auto_increment,
receiver_uid varchar(20),
msg_id varchar(70),
send_time date,
send_uid varchar(20),
msg_type int,
msg_content varchar(1024),
primary key(uuid)
)default charset=utf8;

