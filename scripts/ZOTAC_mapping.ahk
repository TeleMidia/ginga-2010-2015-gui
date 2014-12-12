OnMessage(0x219, "notify_change") 
Return 

DriveGet, Devices, List, REMOVABLE

parse_devices:
	DriveGet, Current_Devices, List, REMOVABLE
	xml_file = C:/tmp/gingagui/usb.xml
	xml_lock = C:/tmp/gingagui/usb.xml.lock 
	
	If (StrLen(Devices) < StrLen(Current_Devices))
	{
		FileAppend, , %xml_lock% 
		Loop, parse, Current_Devices
		{
			If !InStr(Devices, A_LoopField)
			{
				DriveGet, Name, Label, %A_LoopField%:
				init = 0
				xml = ""
				
				Loop, %A_LoopField%:\*.ncl, , 1  ; Recurse into subfolders.
				{
					if (init == 0)
					{
						xml = <?xml version="1.0" encoding="UTF-8"?><rss xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" version="2.0"><channel> <title>NCL applications in this USB Drive:</title>
						init = 1
					}
					xml = %xml%<item><title>%A_LoopFileName%</title><link>%A_LoopFileFullPath%</link></item>
					
				}
				if (init == 1)
				{
					xml = %xml%</channel></rss>
					FileDelete %xml_file% 
					FileAppend, %xml%, %xml_file% 
				}
			}
		}
		FileDelete %xml_lock% 
	}
	If (StrLen(Current_Devices) < StrLen(Devices))
	{
		FileDelete %xml_file% 
	}
	Devices := Current_Devices
	return

notify_change(wParam, lParam, msg, hwnd) 
{ 
	if (wParam == 32768)
	{
		;MsgBox, "USB Inserted"
		Gosub, parse_devices
	}
	else if (wParam == 32772)
	{
		;MsgBox, "USB Removed"
		Gosub, parse_devices
	}
}

;Zotac control remap script
#IfWinActive, Ginga GUI
	;red
	^!1::
	Send {F1}
	return

	;green
	^!2::
	Send {F2}
	return

	;yellow 
	^!3::
	Send {F3}
	return

	;blue
	^!4::
	Send {F4}
	return
	
	;info
	^D::
	Send {F6}
	return
#IfWinActive
