#ifdef ODM_GEMTEK
static void telnet_reset_message(struct work_struct *work)
{

       char cmdPath[]="/bin/sh";
       char* cmdEnvp[]={"HOME=/","PATH=/sbin:/bin:/usr/bin",NULL};
       char str_reset_time[12];
       char str_restore__to_default_time[12];
       char* cmdArgv[]={cmdPath,"/etc/gtk_rbt_msg.sh",str_reset_time,str_restore__to_default_time,NULL};
       //int i=0;

       //convert int to char*
       snprintf(str_reset_time,12,"%d",RESET_TIME);
       snprintf(str_restore__to_default_time,12,"%d",RESTORE__TO_DEFAULT_TIME);
/*
       //allocate cmdArgv memory size and assign value to cmdArgv
       cmdArgv[0] = kmalloc(sizeof(cmdPath),GFP_KERNEL);
       cmdArgv[1] = kmalloc(sizeof("/etc/gtk_rbt_msg.sh"),GFP_KERNEL);
       cmdArgv[2] = kmalloc(sizeof(str_reset_time),GFP_KERNEL);
       cmdArgv[3] = kmalloc(sizeof(str_restore__to_default_time),GFP_KERNEL);
       cmdArgv[4] = NULL;
       snprintf(cmdArgv[0],sizeof(cmdPath),"%s",cmdPath);
       snprintf(cmdArgv[1],sizeof("/etc/gtk_rbt_msg.sh"),"%s","/etc/gtk_rbt_msg.sh");
       snprintf(cmdArgv[2],sizeof(str_reset_time),"%s",str_reset_time);
       snprintf(cmdArgv[3],sizeof(str_restore__to_default_time),"%s",str_restore__to_default_time);
*/
       call_usermodehelper(cmdPath,cmdArgv,cmdEnvp,UMH_WAIT_PROC);
/*
       //free allocated memory(cmdArgv)
       for(i=0;i<4;i++)
           kfree(cmdArgv[i]);
*/
       return;
}
#endif //ODM_GEMTEK

