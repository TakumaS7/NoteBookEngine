/*===============================================================*/
//
//	input.h(“ü—Íˆ—)
// 
// 
// ---------------------------------- 2025/09/25 »ìÒF²“¡‘ô–
//
/*===============================================================*/

#ifndef _INPUT_H
#define _INPUT_H

class Input
{
private:
	static BYTE m_OldKeyState[256];
	static BYTE m_KeyState[256];

public:
	static void Init();
	static void Uninit();
	static void Update();

	static bool GetKeyPress( BYTE KeyCode );
	static bool GetKeyTrigger( BYTE KeyCode );

};

#endif //_INPUT_H