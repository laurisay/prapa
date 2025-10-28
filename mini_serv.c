#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void	ft_err(char *s){write(2,s,strlen(s));exit(1);}

int	extract_msg(char **buf, char **msg)
{
		int i=0; char *new;
			if(!*buf)return 0;
				while((*buf)[i])
						{
									if((*buf)[i++]=='\n')
												{
																if(!(new=calloc(1,strlen(*buf+i)+1)))ft_err("Fatal error\n");
																			strcpy(new,*buf+i);
																						(*buf)[i]=0;
																									*msg=*buf;
																												*buf=new;
																															return 1;
																																	}
										}
					return 0;
}

char	*str_join(char *a, char *b)
{
		char *res; int la=a?strlen(a):0;
			if(!(res=malloc(la+strlen(b)+1)))ft_err("Fatal error\n");
				res[0]=0;
					if(a)strcat(res,a);
						strcat(res,b);
							free(a);
								return res;
}

int	create_sock(int port)
{
		int fd; struct sockaddr_in s;
			if((fd=socket(AF_INET,SOCK_STREAM,0))<0)return -1;
				bzero(&s,sizeof(s));
					s.sin_family=AF_INET;
						s.sin_addr.s_addr=htonl(2130706433);
							s.sin_port=htons(port);
								if(bind(fd,(void*)&s,sizeof(s))<0||listen(fd,10)<0)return -1;
									return fd;
}

void	send_all(int *cli,int max,int skip,char *msg)
{
		for(int i=0;i<max;i++)
					if(cli[i]!=-1&&cli[i]!=skip)
									send(cli[i],msg,strlen(msg),0);
}

int	main(int ac,char **av)
{
		if(ac!=2)ft_err("Wrong number of arguments\n");
			int	port=atoi(av[1]); if(port<=0)ft_err("Wrong number of arguments\n");
				int srv=create_sock(port); if(srv<0)ft_err("Fatal error\n");
					int cli[1024],id[1024],max=1024,ids=0; char *buf[1024]={0};
						for(int i=0;i<max;i++)cli[i]=-1;
							fd_set r;
								while(1)
										{
													FD_ZERO(&r);
															FD_SET(srv,&r);
																	int m=srv;
																			for(int i=0;i<max;i++)
																							if(cli[i]!=-1){FD_SET(cli[i],&r);if(cli[i]>m)m=cli[i];}
																					if(select(m+1,&r,0,0,0)<0)continue;
																							if(FD_ISSET(srv,&r))
																										{
																														struct sockaddr_in c; socklen_t l=sizeof(c);
																																	int fd=accept(srv,(void*)&c,&l);
																																				if(fd>=0)
																																								{
																																													int s=-1;
																																																	for(int i=0;i<max;i++)
																																																						{
																																																												if(cli[i]==-1){s=i;break;}
																																																																}
																																																					if(s==-1)
																																																											close(fd);
																																																									else
																																																														{
																																																																				cli[s]=fd; id[s]=ids++;
																																																																									char msg[64];
																																																																														sprintf(msg,"server: client %d just arrived\n",id[s]);
																																																																																			send_all(cli,max,fd,msg);
																																																																																							}
																																																												}
																																						}
																									for(int i=0;i<max;i++)
																												{
																																if(cli[i]!=-1&&FD_ISSET(cli[i],&r))
																																				{
																																									char tmp[4096];
																																													int n=recv(cli[i],tmp,4095,0);
																																																	if(n<=0)
																																																						{
																																																												char msg[64];
																																																																	sprintf(msg,"server: client %d just left\n",id[i]);
																																																																						send_all(cli,max,cli[i],msg);
																																																																											close(cli[i]);
																																																																																cli[i]=-1;
																																																																																					free(buf[i]);
																																																																																										buf[i]=0;
																																																																																														}
																																																					else
																																																										{
																																																																tmp[n]=0;
																																																																					buf[i]=str_join(buf[i],tmp);
																																																																										char *msg;
																																																																															while(extract_msg(&buf[i],&msg))
																																																																																					{
																																																																																												char pre[64];
																																																																																																		sprintf(pre,"client %d: ",id[i]);
																																																																																																								char *full=malloc(strlen(pre)+strlen(msg)+1);
																																																																																																														if(!full)ft_err("Fatal error\n");
																																																																																																																				strcpy(full,pre);
																																																																																																																										strcat(full,msg);
																																																																																																																																send_all(cli,max,cli[i],full);
																																																																																																																																						free(msg);
																																																																																																																																												free(full);
																																																																																																																																																	}
																																																																																			}
																																																								}
																																		}
																										}
}

