B4A=true
Group=Default Group
ModulesStructureVersion=1
Type=Service
Version=5.18
@EndOfDesignText@
#Region  Service Attributes 
	#StartAtBoot: False
	#ExcludeFromLibrary: True
#End Region

Sub Process_Globals
	'These global variables will be declared once when the application starts.
	'These variables can be accessed from all modules.
	Public kvs As KeyValueStore
End Sub


Sub Service_Create
	kvs.Initialize(File.DirInternal, "datastore2")
End Sub


Sub Service_Start (StartingIntent As Intent)
	
End Sub

Sub Service_Destroy

End Sub
