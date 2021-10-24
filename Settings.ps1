# Configure the variables below that will be used in the script
$HAToken = "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJpc3MiOiI5NzFlNzcyNzFiNjA0MDg1ODAwYzY5NzQ2YTZlMDc0ZCIsImlhdCI6MTYzMDA2NDg2MywiZXhwIjoxOTQ1NDI0ODYzfQ.T1-PXFLR-yCAfaZ-TALlQpqF9_zRgtnDxllMdcNfTZ8" # Example: eyJ0eXAiOiJKV1...
$UserName = "H960794" # When not sure, open a command prompt and type: echo %USERNAME%
$HAUrl = "https://tuvzrahx4hb2tdi2ova5sbu7z0raa2lm.ui.nabu.casa" # Example: https://yourha.duckdns.org

# Set language variables below
$lgAvailable = "Available"
$lgBusy = "Busy"
$lgOnThePhone = "On the phone"
$lgAway = "Away"
$lgBeRightBack = "Be right back"
$lgDoNotDisturb = "Do not disturb"
$lgPresenting = "Presenting"
$lgFocusing = "Focusing"
$lgInAMeeting = "In a meeting"
$lgOffline = "Offline"
$lgNotInACall = "Not in a call"
$lgInACall = "In a call"
$lgCalling = "calling"


# Set icons to use for call activity
$iconInACall = "mdi:phone-in-talk-outline"
$iconNotInACall = "mdi:phone-off"
$iconMonitoring = "mdi:api"

# Set entities to post to
$entityStatus = "sensor.teams_status"
$entityStatusName = "Microsoft Teams status"
$entityActivity = "sensor.teams_activity"
$entityActivityName = "Microsoft Teams activity"
$entityHeartbeat = "binary_sensor.teams_monitoring"
$entityHeartbeatName = "Microsoft Teams monitoring"
