use im;
create table if not exists User
(
uid varchar(20),
nickname varchar(20) default 'empty',
account varchar(30) not null,
password varchar(15) not null,
primary key(uid)
)default charset=utf8;

create table if not exists Friend
(
id int unsigned auto_increment,
f1_uid varchar(20) not null,
f2_uid varchar(20) not null,
primary key(id)
)default charset=utf8;



