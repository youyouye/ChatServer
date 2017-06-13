use im;
create table if not exists GroupUsers
(
group_id varchar(20),
user_id varchar(20),
lask_ack_msg_id varchar(70),
primary key(group_id,user_id)
)default charset=utf8;

create table if not exists GroupMsgs
(
group_id varchar(20),
sender_id varchar(20),
msg_id varchar(70),
msg_type int,
msg_content varchar(1024),
primary key(group_id,msg_id)
)default charset=utf8;

