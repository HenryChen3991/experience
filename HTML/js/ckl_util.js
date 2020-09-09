function colorLog(message, color) {
	color = color || "black";
	switch (color) {
		case "green":  
			 color = "Green"; 
			 break;
		case "blue":     
			 color = "DodgerBlue";  
			 break;
		case "red":   
			 color = "Red";     
			 break;
		case "orange":  
			 color = "Orange";   
			 break;
		default: 
			 color = color;
	}
	console.log("%c" + message, "color:" + color);
}
//get function name
//example:
//	getFunctionName(arguments.callee.toString())
function getFunctionName(funcName){
	//console.log(me);
	funcName = funcName.substr('function '.length);   
	funcName = funcName.substr(0, funcName.indexOf('(')); 
	return funcName;
}