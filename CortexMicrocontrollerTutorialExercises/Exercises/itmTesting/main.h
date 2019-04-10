
typedef struct _MailMessage {
	uint8_t ledLowerNibble;
	uint8_t ledUpperNibble;
	} MailMessage;
#ifdef _main
OS_TID t_Task1;
OS_TID t_Task2;
OS_TID t_Task3;
OS_TID t_Error;
OS_TID t_Test;
extern __task void errorHandler (void);
//extern __task void testTask(void);

os_mbx_declare(MsgBoxTask2,10);           									            
_declare_box(mpoolTask2, sizeof(MailMessage), 10);						

#else 
extern 	int SER_GetChar (void);
extern os_mbx_declare(MsgBoxTask2,10);           									            
extern _declare_box(mpoolTask2, sizeof(MailMessage), 10);	 
extern __task void Task1 (void);
extern __task void Task2 (void);
extern OS_TID t_Task1;
extern OS_TID t_Task2;
extern OS_TID t_Task3;
#endif

