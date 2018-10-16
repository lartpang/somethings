#include	<stdio.h>
#include	<stdlib.h>
#include	<windows.h>
#include	"WinParams.h"
#include	"WinImage.h"

BOOL CALLBACK DlgProc1(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK DlgProc2(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK DlgProc3(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK DlgProc4(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK DlgProc5(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK DlgProc6(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK DlgProc8(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK DlgProcS2(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK DlgProcS3(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK DlgProcT(HWND, UINT, WPARAM, LPARAM);

extern	HANDLE  ghInst;
extern	HWND	hGWnd;
int		gMode;
char	gQuestion[128];
char	gQuestion1[128], gQuestion2[128], gQuestion3[128];
char	gQuestion4[128], gQuestion5[128], gQuestion6[128];
char	gQuestion7[128], gQuestion8[128];
char	gAnswer1[128], gAnswer2[128], gAnswer3[128];
char	gAnswer4[128], gAnswer5[128], gAnswer6[128];
char	gAnswer7[128], gAnswer8[128];
char	gResult[10000];

/*--- GetParams1 --- �p�����[�^�𓾂� -----------------------------------------
	szQuestion:		����
	param1:			�p�����[�^�P
-----------------------------------------------------------------------------*/
BOOL GetParams1(char *szQuestion, double *param1)
{
	sprintf(gQuestion, szQuestion);
	if (DialogBox(ghInst, "Dialog_1", hGWnd, DlgProc1) != TRUE) return FALSE;
	*param1 = atof(gAnswer1);
	return TRUE;
}

/*--- DlgProc1 --- �_�C�A���O���������� ---------------------------------------
	hDlg:		�E�B���h�E
	message:	���b�Z�[�W
	wParam:		���[�h�p�����[�^
	lParam:		�����O�p�����[�^
-----------------------------------------------------------------------------*/
BOOL CALLBACK DlgProc1(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
        case WM_INITDIALOG:
            SetDlgItemText (hDlg, IDC_EDIT1, (LPSTR) gQuestion);
           return TRUE;
        case WM_COMMAND:
            switch (wParam) {
                case IDOK:
            		GetDlgItemText (hDlg, IDC_EDIT2, (LPSTR) gAnswer1, 16);
                    EndDialog (hDlg, TRUE);
                    return TRUE;
                case IDCANCEL:
					EndDialog (hDlg, FALSE);
                    return FALSE;
                default:
                    break;
            }
    }
    return FALSE;
}

/*--- GetParams2 --- �p�����[�^�𓾂� -----------------------------------------
	szQuestion:		����
	szQuestion1:	����
	szQuestion2:	����
	param1:			�p�����[�^�P
	param2:			�p�����[�^�Q
-----------------------------------------------------------------------------*/
BOOL GetParams2(char *szQuestion,char *szQuestion1,char *szQuestion2,
	double *param1, double *param2)
{
	sprintf(gQuestion, szQuestion);
	sprintf(gQuestion1, szQuestion1);
	sprintf(gQuestion2, szQuestion2);
	if (DialogBox(ghInst, "Dialog_2", hGWnd, DlgProc2) != TRUE) return FALSE;
	*param1 = atof(gAnswer1);
	*param2 = atof(gAnswer2);
	return TRUE;
}

/*--- DlgProc2 --- �_�C�A���O���������� ---------------------------------------
	hDlg:		�E�B���h�E
	message:	���b�Z�[�W
	wParam:		���[�h�p�����[�^
	lParam:		�����O�p�����[�^
-----------------------------------------------------------------------------*/
BOOL CALLBACK DlgProc2(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
        case WM_INITDIALOG:
				SetDlgItemText (hDlg, IDC_EDIT1, (LPSTR) gQuestion);
				SetDlgItemText (hDlg, IDC_EDIT2, (LPSTR) gQuestion1);
				SetDlgItemText (hDlg, IDC_EDIT4, (LPSTR) gQuestion2);
				return TRUE;
        case WM_COMMAND:
            switch (wParam) {
                case IDOK:
            		GetDlgItemText (hDlg, IDC_EDIT3, (LPSTR) gAnswer1, 16);
            		GetDlgItemText (hDlg, IDC_EDIT5, (LPSTR) gAnswer2, 16);
                    EndDialog (hDlg, TRUE);
                    return TRUE;
                case IDCANCEL:
					EndDialog (hDlg, FALSE);
                    return FALSE;
                default:
                    break;
            }
    }
    return FALSE;
}

/*--- GetParams3 --- �p�����[�^�𓾂� -----------------------------------------
	szQuestion:		����
	szQuestion1:	����P
	szQuestion2:	����Q
	szQuestion3:	����R
	param1:			�p�����[�^�P
	param2:			�p�����[�^�Q
	param3:			�p�����[�^�R
-----------------------------------------------------------------------------*/
BOOL GetParams3(char *szQuestion,char *szQuestion1,char *szQuestion2,
	char *szQuestion3, double *param1, double *param2, double *param3)
{
	sprintf(gQuestion, szQuestion);
	sprintf(gQuestion1, szQuestion1);
	sprintf(gQuestion2, szQuestion2);
	sprintf(gQuestion3, szQuestion3);
	if (DialogBox(ghInst, "Dialog_3", hGWnd, DlgProc3) != TRUE) return FALSE;
	*param1 = atof(gAnswer1);
	*param2 = atof(gAnswer2);
	*param3 = atof(gAnswer3);
	return TRUE;
}

/*--- DlgProc3 --- �_�C�A���O���������� ---------------------------------------
	hDlg:		�E�B���h�E
	message:	���b�Z�[�W
	wParam:		���[�h�p�����[�^
	lParam:		�����O�p�����[�^
-----------------------------------------------------------------------------*/
BOOL CALLBACK DlgProc3(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
        case WM_INITDIALOG:
				SetDlgItemText (hDlg, IDC_EDIT1, (LPSTR) gQuestion);
				SetDlgItemText (hDlg, IDC_EDIT2, (LPSTR) gQuestion1);
				SetDlgItemText (hDlg, IDC_EDIT4, (LPSTR) gQuestion2);
				SetDlgItemText (hDlg, IDC_EDIT6, (LPSTR) gQuestion3);
				return TRUE;
        case WM_COMMAND:
            switch (wParam) {
                case IDOK:
            		GetDlgItemText (hDlg, IDC_EDIT3, (LPSTR) gAnswer1, 16);
            		GetDlgItemText (hDlg, IDC_EDIT5, (LPSTR) gAnswer2, 16);
            		GetDlgItemText (hDlg, IDC_EDIT7, (LPSTR) gAnswer3, 16);
                    EndDialog (hDlg, TRUE);
                    return TRUE;
                case IDCANCEL:
					EndDialog (hDlg, FALSE);
                    return FALSE;
                default:
                    break;
            }
    }
    return FALSE;
}

/*--- GetParams4 --- �p�����[�^�𓾂� -----------------------------------------
	szQuestion:		����
	szQuestion1:	����P
	szQuestion2:	����Q
	szQuestion3:	����R
	szQuestion4:	����S
	param1:			�p�����[�^�P
	param2:			�p�����[�^�Q
	param3:			�p�����[�^�R
	param4:			�p�����[�^�S
-----------------------------------------------------------------------------*/
BOOL GetParams4(char *szQuestion, char *szQuestion1, char *szQuestion2,
	char *szQuestion3, char *szQuestion4,
	double *param1, double *param2, double *param3, double *param4)
{
	sprintf(gQuestion, szQuestion);
	sprintf(gQuestion1, szQuestion1);
	sprintf(gQuestion2, szQuestion2);
	sprintf(gQuestion3, szQuestion3);
	sprintf(gQuestion4, szQuestion4);
	if (DialogBox(ghInst, "Dialog_4", hGWnd, DlgProc4) != TRUE) return FALSE;
	*param1 = atof(gAnswer1);
	*param2 = atof(gAnswer2);
	*param3 = atof(gAnswer3);
	*param4 = atof(gAnswer4);
	return TRUE;
}

/*--- DlgProc4 --- �_�C�A���O���������� ---------------------------------------
	hDlg:		�E�B���h�E
	message:	���b�Z�[�W
	wParam:		���[�h�p�����[�^
	lParam:		�����O�p�����[�^
-----------------------------------------------------------------------------*/
BOOL CALLBACK DlgProc4(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
        case WM_INITDIALOG:
				SetDlgItemText (hDlg, IDC_EDIT1, (LPSTR) gQuestion);
				SetDlgItemText (hDlg, IDC_EDIT2, (LPSTR) gQuestion1);
				SetDlgItemText (hDlg, IDC_EDIT4, (LPSTR) gQuestion2);
				SetDlgItemText (hDlg, IDC_EDIT6, (LPSTR) gQuestion3);
				SetDlgItemText (hDlg, IDC_EDIT8, (LPSTR) gQuestion4);
				return TRUE;
        case WM_COMMAND:
            switch (wParam) {
                case IDOK:
            		GetDlgItemText (hDlg, IDC_EDIT3, (LPSTR) gAnswer1, 16);
            		GetDlgItemText (hDlg, IDC_EDIT5, (LPSTR) gAnswer2, 16);
            		GetDlgItemText (hDlg, IDC_EDIT7, (LPSTR) gAnswer3, 16);
            		GetDlgItemText (hDlg, IDC_EDIT9, (LPSTR) gAnswer4, 16);
                    EndDialog (hDlg, TRUE);
                    return TRUE;
                case IDCANCEL:
					EndDialog (hDlg, FALSE);
                    return FALSE;
                default:
                    break;
            }
    }
    return FALSE;
}

/*--- GetParams5 --- �p�����[�^�𓾂� -----------------------------------------
	szQuestion:		����
	szQuestion1:	����P
	szQuestion2:	����Q
	szQuestion3:	����R
	szQuestion4:	����S
	szQuestion5:	����T
	param1:			�p�����[�^�P
	param2:			�p�����[�^�Q
	param3:			�p�����[�^�R
	param4:			�p�����[�^�S
	param5:			�p�����[�^�T
-----------------------------------------------------------------------------*/
BOOL GetParams5(char *szQuestion, char *szQuestion1, char *szQuestion2,
	char *szQuestion3, char *szQuestion4, char *szQuestion5,
	double *param1, double *param2, double *param3,
	double *param4, double *param5)
{
	sprintf(gQuestion, szQuestion);
	sprintf(gQuestion1, szQuestion1);
	sprintf(gQuestion2, szQuestion2);
	sprintf(gQuestion3, szQuestion3);
	sprintf(gQuestion4, szQuestion4);
	sprintf(gQuestion5, szQuestion5);
	if (DialogBox(ghInst, "Dialog_5", hGWnd, DlgProc5) != TRUE) return FALSE;
	*param1 = atof(gAnswer1);
	*param2 = atof(gAnswer2);
	*param3 = atof(gAnswer3);
	*param4 = atof(gAnswer4);
	*param5 = atof(gAnswer5);
	return TRUE;
}

/*--- DlgProc5 --- �_�C�A���O���������� ---------------------------------------
	hDlg:		�E�B���h�E
	message:	���b�Z�[�W
	wParam:		���[�h�p�����[�^
	lParam:		�����O�p�����[�^
-----------------------------------------------------------------------------*/
BOOL CALLBACK DlgProc5(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
        case WM_INITDIALOG:
				SetDlgItemText (hDlg, IDC_EDIT1, (LPSTR) gQuestion);
				SetDlgItemText (hDlg, IDC_EDIT2, (LPSTR) gQuestion1);
				SetDlgItemText (hDlg, IDC_EDIT4, (LPSTR) gQuestion2);
				SetDlgItemText (hDlg, IDC_EDIT6, (LPSTR) gQuestion3);
				SetDlgItemText (hDlg, IDC_EDIT8, (LPSTR) gQuestion4);
				SetDlgItemText (hDlg, IDC_EDIT10, (LPSTR) gQuestion5);
				return TRUE;
        case WM_COMMAND:
            switch (wParam) {
                case IDOK:
            		GetDlgItemText (hDlg, IDC_EDIT3, (LPSTR) gAnswer1, 16);
            		GetDlgItemText (hDlg, IDC_EDIT5, (LPSTR) gAnswer2, 16);
            		GetDlgItemText (hDlg, IDC_EDIT7, (LPSTR) gAnswer3, 16);
            		GetDlgItemText (hDlg, IDC_EDIT9, (LPSTR) gAnswer4, 16);
            		GetDlgItemText (hDlg, IDC_EDIT11, (LPSTR) gAnswer5, 16);
                    EndDialog (hDlg, TRUE);
                    return TRUE;
                case IDCANCEL:
					EndDialog (hDlg, FALSE);
                    return FALSE;
                default:
                    break;
            }
    }
    return FALSE;
}

/*--- GetParams6 --- �p�����[�^�𓾂� -----------------------------------------
	szQuestion:		����
	szQuestion1:	����P
	szQuestion2:	����Q
	szQuestion3:	����R
	szQuestion4:	����S
	szQuestion5:	����T
	szQuestion6:	����U
	param1:			�p�����[�^�P
	param2:			�p�����[�^�Q
	param3:			�p�����[�^�R
	param4:			�p�����[�^�S
	param5:			�p�����[�^�T
	param6:			�p�����[�^�U
-----------------------------------------------------------------------------*/
BOOL GetParams6(char *szQuestion, char *szQuestion1, char *szQuestion2,
	char *szQuestion3, char *szQuestion4, char *szQuestion5, char *szQuestion6,
	double *param1, double *param2, double *param3, double *param4,
	double *param5, double *param6)
{
	sprintf(gQuestion, szQuestion);
	sprintf(gQuestion1, szQuestion1);
	sprintf(gQuestion2, szQuestion2);
	sprintf(gQuestion3, szQuestion3);
	sprintf(gQuestion4, szQuestion4);
	sprintf(gQuestion5, szQuestion5);
	sprintf(gQuestion6, szQuestion6);
	if (DialogBox(ghInst, "Dialog_6", hGWnd, DlgProc6) != TRUE) return FALSE;
	*param1 = atof(gAnswer1);
	*param2 = atof(gAnswer2);
	*param3 = atof(gAnswer3);
	*param4 = atof(gAnswer4);
	*param5 = atof(gAnswer5);
	*param6 = atof(gAnswer6);
	return TRUE;
}

/*--- DlgProc6 --- �_�C�A���O���������� ---------------------------------------
	hDlg:		�E�B���h�E
	message:	���b�Z�[�W
	wParam:		���[�h�p�����[�^
	lParam:		�����O�p�����[�^
-----------------------------------------------------------------------------*/
BOOL CALLBACK DlgProc6(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
        case WM_INITDIALOG:
				SetDlgItemText (hDlg, IDC_EDIT1, (LPSTR) gQuestion);
				SetDlgItemText (hDlg, IDC_EDIT2, (LPSTR) gQuestion1);
				SetDlgItemText (hDlg, IDC_EDIT4, (LPSTR) gQuestion2);
				SetDlgItemText (hDlg, IDC_EDIT6, (LPSTR) gQuestion3);
				SetDlgItemText (hDlg, IDC_EDIT8, (LPSTR) gQuestion4);
				SetDlgItemText (hDlg, IDC_EDIT10, (LPSTR) gQuestion5);
				SetDlgItemText (hDlg, IDC_EDIT12, (LPSTR) gQuestion6);
				return TRUE;
        case WM_COMMAND:
            switch (wParam) {
                case IDOK:
            		GetDlgItemText (hDlg, IDC_EDIT3, (LPSTR) gAnswer1, 16);
            		GetDlgItemText (hDlg, IDC_EDIT5, (LPSTR) gAnswer2, 16);
            		GetDlgItemText (hDlg, IDC_EDIT7, (LPSTR) gAnswer3, 16);
            		GetDlgItemText (hDlg, IDC_EDIT9, (LPSTR) gAnswer4, 16);
            		GetDlgItemText (hDlg, IDC_EDIT11, (LPSTR) gAnswer5, 16);
            		GetDlgItemText (hDlg, IDC_EDIT13, (LPSTR) gAnswer6, 16);
                    EndDialog (hDlg, TRUE);
                    return TRUE;
                case IDCANCEL:
					EndDialog (hDlg, FALSE);
                    return FALSE;
                default:
                    break;
            }
    }
    return FALSE;
}

/*--- GetParams8 --- �p�����[�^�𓾂� -----------------------------------------
	szQuestion:		����
	szQuestion1:	����P
	szQuestion2:	����Q
	szQuestion3:	����R
	szQuestion4:	����S
	szQuestion5:	����T
	szQuestion6:	����U
	szQuestion7:	����V
	szQuestion8:	����W
	param1:			�p�����[�^�P
	param2:			�p�����[�^�Q
	param3:			�p�����[�^�R
	param4:			�p�����[�^�S
	param5:			�p�����[�^�T
	param6:			�p�����[�^�U
	param7:			�p�����[�^�V
	param8:			�p�����[�^�W
-----------------------------------------------------------------------------*/
BOOL GetParams8(char *szQuestion, char *szQuestion1, char *szQuestion2,
	char *szQuestion3, char *szQuestion4, char *szQuestion5, char *szQuestion6,
	char *szQuestion7, char *szQuestion8,
	double *param1, double *param2, double *param3, double *param4,
	double *param5, double *param6, double *param7, double *param8)
{
	sprintf(gQuestion, szQuestion);
	sprintf(gQuestion1, szQuestion1);
	sprintf(gQuestion2, szQuestion2);
	sprintf(gQuestion3, szQuestion3);
	sprintf(gQuestion4, szQuestion4);
	sprintf(gQuestion5, szQuestion5);
	sprintf(gQuestion6, szQuestion6);
	sprintf(gQuestion7, szQuestion7);
	sprintf(gQuestion8, szQuestion8);
	if (DialogBox(ghInst, "Dialog_8", hGWnd, DlgProc8) != TRUE) return FALSE;
	*param1 = atof(gAnswer1);
	*param2 = atof(gAnswer2);
	*param3 = atof(gAnswer3);
	*param4 = atof(gAnswer4);
	*param5 = atof(gAnswer5);
	*param6 = atof(gAnswer6);
	*param7 = atof(gAnswer7);
	*param8 = atof(gAnswer8);
	return TRUE;
}

/*--- DlgProc8 --- �_�C�A���O���������� ---------------------------------------
	hDlg:		�E�B���h�E
	message:	���b�Z�[�W
	wParam:		���[�h�p�����[�^
	lParam:		�����O�p�����[�^
-----------------------------------------------------------------------------*/
BOOL CALLBACK DlgProc8(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
        case WM_INITDIALOG:
				SetDlgItemText (hDlg, IDC_EDIT1, (LPSTR) gQuestion);
				SetDlgItemText (hDlg, IDC_EDIT2, (LPSTR) gQuestion1);
				SetDlgItemText (hDlg, IDC_EDIT4, (LPSTR) gQuestion2);
				SetDlgItemText (hDlg, IDC_EDIT6, (LPSTR) gQuestion3);
				SetDlgItemText (hDlg, IDC_EDIT8, (LPSTR) gQuestion4);
				SetDlgItemText (hDlg, IDC_EDIT10, (LPSTR) gQuestion5);
				SetDlgItemText (hDlg, IDC_EDIT12, (LPSTR) gQuestion6);
				SetDlgItemText (hDlg, IDC_EDIT14, (LPSTR) gQuestion7);
				SetDlgItemText (hDlg, IDC_EDIT16, (LPSTR) gQuestion8);
				return TRUE;
        case WM_COMMAND:
            switch (wParam) {
                case IDOK:
            		GetDlgItemText (hDlg, IDC_EDIT3, (LPSTR) gAnswer1, 16);
            		GetDlgItemText (hDlg, IDC_EDIT5, (LPSTR) gAnswer2, 16);
            		GetDlgItemText (hDlg, IDC_EDIT7, (LPSTR) gAnswer3, 16);
            		GetDlgItemText (hDlg, IDC_EDIT9, (LPSTR) gAnswer4, 16);
            		GetDlgItemText (hDlg, IDC_EDIT11, (LPSTR) gAnswer5, 16);
            		GetDlgItemText (hDlg, IDC_EDIT13, (LPSTR) gAnswer6, 16);
            		GetDlgItemText (hDlg, IDC_EDIT15, (LPSTR) gAnswer7, 16);
            		GetDlgItemText (hDlg, IDC_EDIT17, (LPSTR) gAnswer8, 16);
                    EndDialog (hDlg, TRUE);
                    return TRUE;
                case IDCANCEL:
					EndDialog (hDlg, FALSE);
                    return FALSE;
                default:
                    break;
            }
    }
    return FALSE;
}

/*--- GetSelection2 --- �I������������ ----------------------------------------
	szQuestion:		����
	szQuestion1:	�I���P
	szQuestion2:	�I���Q
	param:			����(1,2)
-----------------------------------------------------------------------------*/
BOOL GetSelection2(char *szQuestion,char *szQuestion1,char *szQuestion2,
	int *param)
{
	sprintf(gQuestion, szQuestion);
	sprintf(gQuestion1, szQuestion1);
	sprintf(gQuestion2, szQuestion2);
	if (DialogBox(ghInst, "Dialog_S2", hGWnd, DlgProcS2) != TRUE) return FALSE;
	*param = gMode;
	return TRUE;
}

/*--- DlgProcS2 --- �_�C�A���O���������� --------------------------------------
	hDlg:		�E�B���h�E
	message:	���b�Z�[�W
	wParam:		���[�h�p�����[�^
	lParam:		�����O�p�����[�^
-----------------------------------------------------------------------------*/
BOOL CALLBACK DlgProcS2(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
        case WM_INITDIALOG:
			SetDlgItemText (hDlg, IDC_EDIT1, (LPSTR) gQuestion);
			SetDlgItemText (hDlg, IDC_RADIO1, (LPSTR) gQuestion1);
			SetDlgItemText (hDlg, IDC_RADIO2, (LPSTR) gQuestion2);
			CheckRadioButton (hDlg, IDC_RADIO1,IDC_RADIO2, IDC_RADIO1);
			gMode = 1;
			return TRUE;
        case WM_COMMAND:
            switch (wParam)	{
                case IDOK:
					if (IsDlgButtonChecked (hDlg, IDC_RADIO1)) gMode =1;
					if (IsDlgButtonChecked (hDlg, IDC_RADIO2)) gMode =2;
					EndDialog (hDlg, TRUE);
                    return TRUE;
                case IDCANCEL:
					EndDialog (hDlg, FALSE);
                    return FALSE;
                default:
                    break;
            }
    }
    return FALSE;
}

/*--- GetSelection3 --- �I������������ ----------------------------------------
	szQuestion:		����
	szQuestion1:	�I���P
	szQuestion2:	�I���Q
	szQuestion3:	�I���R
	param:			����(1,2,3)
-----------------------------------------------------------------------------*/
BOOL GetSelection3(char *szQuestion,char *szQuestion1,char *szQuestion2,
	char *szQuestion3,int *param)
{
	sprintf(gQuestion, szQuestion);
	sprintf(gQuestion1, szQuestion1);
	sprintf(gQuestion2, szQuestion2);
	sprintf(gQuestion3, szQuestion3);
	if (DialogBox(ghInst, "Dialog_S3", hGWnd, DlgProcS3) != TRUE) return FALSE;
	*param = gMode;
	return TRUE;
}

/*--- DlgProcS3 --- �_�C�A���O���������� --------------------------------------
	hDlg:		�E�B���h�E
	message:	���b�Z�[�W
	wParam:		���[�h�p�����[�^
	lParam:		�����O�p�����[�^
-----------------------------------------------------------------------------*/
BOOL CALLBACK DlgProcS3(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
        case WM_INITDIALOG:
			SetDlgItemText (hDlg, IDC_EDIT1, (LPSTR) gQuestion);
			SetDlgItemText (hDlg, IDC_RADIO1, (LPSTR) gQuestion1);
			SetDlgItemText (hDlg, IDC_RADIO2, (LPSTR) gQuestion2);
			SetDlgItemText (hDlg, IDC_RADIO3, (LPSTR) gQuestion3);
			CheckRadioButton (hDlg, IDC_RADIO1, IDC_RADIO3, IDC_RADIO1);
			gMode = 1;
			return TRUE;
        case WM_COMMAND:
            switch (wParam) {
                case IDOK:
					if (IsDlgButtonChecked (hDlg, IDC_RADIO1)) gMode =1;
					if (IsDlgButtonChecked (hDlg, IDC_RADIO2)) gMode =2;
					if (IsDlgButtonChecked (hDlg, IDC_RADIO3)) gMode =3;
					EndDialog (hDlg, TRUE);
                    return TRUE;
                case IDCANCEL:
					EndDialog (hDlg, FALSE);
                    return FALSE;
                default:
                    break;
            }
    }
    return FALSE;
}

/*--- ShowText --- �e�L�X�g��\������ ------------------------------------------
	szQuestion:		�^�C�g��
	szResult:		����
-----------------------------------------------------------------------------*/
BOOL ShowText(char *szQuestion, char *szResult)
{
	char s;
	int  i, m, posi, len;

	sprintf(gQuestion, "%s", szQuestion);
	posi = 0;
	len = strlen(szResult);
	for (i = 0; i < len; i++) {
		s = szResult[i]; 
		if (s == '\n') m = sprintf(&gResult[posi], "%s", "\r\n");
		else m = sprintf(&gResult[posi], "%c", s);
		posi += m;
	}
	if (DialogBox(ghInst, "Dialog_T", hGWnd, DlgProcT) != TRUE) return FALSE;
	return TRUE;
}

/*--- DlgProcT --- �_�C�A���O���������� ----------------------------------------
	hDlg:		�E�B���h�E
	message:	���b�Z�[�W
	wParam:		���[�h�p�����[�^
	lParam:		�����O�p�����[�^
-----------------------------------------------------------------------------*/
BOOL CALLBACK DlgProcT(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
		case WM_INITDIALOG:
			SetDlgItemText (hDlg, IDC_EDIT1, (LPSTR)gQuestion);
			SetDlgItemText (hDlg, IDC_EDIT2, (LPSTR)gResult);
			return TRUE;
		case WM_COMMAND:
			switch (wParam) {
				case IDOK:
					EndDialog (hDlg, TRUE);
					return TRUE;
                case IDCANCEL:
					EndDialog (hDlg, FALSE);
                    return FALSE;
				default:
					break;
			}
	}
	return FALSE;
}
