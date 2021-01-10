<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
	<meta charset="utf-8"/>

	<link rel="stylesheet" type="text/css" href="css/bootstrap.css">
	<link rel="stylesheet" href="css/message.css">

	<script type="text/javascript" src="js/jquery-3.3.1.min.js"></script>
	<script type="text/javascript" src="js/bootstrap.min.js"></script>
	<script type="text/javascript" src="js/message.js"></script>
	<script src="http://pv.sohu.com/cityjson?ie=utf-8"></script> 
	
<script type="text/javascript">
var numss;
function addPreZero(numss){
	if(numss<10){
		return '0'+numss;
	}else{
		return numss;
	}
}
		var timecheck;
		var cup;
		var x=0;
		var str;
		var ip =     "10.12.33.161";
		var port = "8003";
		var ws = new WebSocket("ws://" + ip + ":" + port + "/echo");
		var myDate = new Date();
        var mm = prompt('你的昵称：(默认"皮卡丘")',"");
        if(mm==''||mm==null){
        	mm = "皮卡丘_" + Math.ceil(Math.random()*1000);
        	cup=$(".mbox").val();
        	x=1;
			}
		else{
			while(mm.length>10){
				mm = prompt("输入的昵称过长(最大10个字符)","");
			};
			mm = mm + "_" + Math.ceil(Math.random()*1000);
			}
		var received_msg;
		var	xxx=1;
		window.onload=function (){
			$(".box").attr("readonly", false);
			$('.box').bind('keypress',function(event){ 
				if(event.keyCode == 13&&xxx==1&&$(".box").val()!="")      
				{  
					xxx=0;
					window.setTimeout("xxx=1;",2000);
					$(".sub").click();
				}  
			});
		}
        //$.message({message:'信息提醒', type:'info'});
// 		message:' 操作成功',    //提示信息
// time:'2000',           //显示时间（默认：2s）
// type:'success',        //显示类型，包括4种：success.error,info,warning
// showClose:false,       //显示关闭按钮（默认：否）
// autoClose:true,        //是否自动关闭（默认：是）
		ws.onopen = function (){
			if (x==1) {
				//window.setTimeout('alert("已为你自动设置昵称："+mm);ws.send("onlykeyforip,"+returnCitySN["cip"]+returnCitySN["cname"]+myDate.toLocaleString()+","+mm);',1000);
				window.setTimeout('$.message({message:"欢迎您！"+mm,type:"success"});ws.send("onlykeyforip,"+returnCitySN["cip"]+returnCitySN["cname"]+myDate.toLocaleString()+","+mm);',1000);
				}
			else{
				//window.setTimeout('alert("已为你自动设置昵称："+mm);ws.send("onlykeyforip,"+returnCitySN["cip"]+returnCitySN["cname"]+myDate.toLocaleString()+","+mm);',1000);
				window.setTimeout('$.message({message:"欢迎您！"+mm,type:"success"});ws.send("onlykeyforip,"+returnCitySN["cip"]+returnCitySN["cname"]+myDate.toLocaleString()+","+mm);',1000);
				}
				window.setTimeout("$('.box').focus();$('.sub').attr('disabled', true);",1000);
		}
		ws.onmessage = function (evt)
        {
			myDate = new Date();
        	received_msg = evt.data;
			if(received_msg.length>32){
				strs=received_msg.split(':');
				if(strs[1]=="在线"){
					$(".online").html("在线"+strs[2]+"人");
				}else{
					cup=$(".mbox").val();
					$(".mbox").val(received_msg+'           '+addPreZero(myDate.getHours())+':'+addPreZero(myDate.getMinutes())+':'+addPreZero(myDate.getSeconds())+'\n'+cup);
				}
			}else{
        		cup=$(".mbox").val();
				$(".mbox").val(received_msg+'           '+addPreZero(myDate.getHours())+':'+addPreZero(myDate.getMinutes())+':'+addPreZero(myDate.getSeconds())+'\n'+cup);
			}
        };
		function sends()
		{	
			
			str = $(".box").val();
			if(str.length>100){
				//alert("发送文本过长!最多100字");
				$.message({message:"发送文本过长!最多100字",type:"warning"});
				}
			else if(str!=""){
				ws.send(mm+":\n        "+str);
				$(".box").val("");
				$(".sub").attr("disabled", true);
				$(".box").val("还剩2秒可再次发言");
				//$.message('还剩2秒可再次发言');
				$(".box").attr("readonly", true);
				window.setTimeout('$(".box").val("还剩1秒可再次发言");',1000);
				//window.setTimeout('$.message("还剩1秒可再次发言");',1000);
				window.setTimeout('$(".box").val("");$(".box").attr("readonly", false);',2000);
				}
			else{
				cup=$(".mbox").val();
				$.message({message:"发送消息不能为空",type:"warning"});
				//$(".mbox").val("发送消息不能为空。"+'\n'+cup);
				}
		}
		function coloregg(){
			if($(".box").val()==""){
			$(".sub").attr("disabled", true);
		}else if($(".box").val()=="技术激情协作创新"){
			ws.send("系统消息：恭喜"+mm+"获得彩蛋！！！");
			//$(".box").val("恭喜你获得彩蛋！");
			$.message({message:"恭喜你获得彩蛋！",type:"success"});
			window.setTimeout('$(".box").val("");',1000);
			window.setTimeout('window.location.href="gamelol.php";',2000);
		}else{
			$(".sub").attr("disabled", false);
		}	
	}
	</script>
	<style type="text/css">
		body{
			background-color:rgb(46,43,38);
			font-family: -apple-system,BlinkMacSystemFont,"Segoe UI",Roboto,"Helvetica Neue",Arial,sans-serif,"Apple Color Emoji","Segoe UI Emoji","Segoe UI Symbol";
		}
		.mbox{
			width: 99.2%;
			height: 600px;
            font-size: 16px;
			color: black;
			border-radius: 5px;
			background-color:wheat;
		}
		.sub{
			
			width: 20%;
			margin-bottom:7px;
		}
		.box{
			background-color:wheat;
			font-family: -apple-system,BlinkMacSystemFont,"Segoe UI",Roboto,"Helvetica Neue",Arial,sans-serif,"Apple Color Emoji","Segoe UI Emoji","Segoe UI Symbol";
			margin-top: 10px;
			width: 78%;
			height: 40px;
			font-size: 16px;
			color: black;
			border-radius: 5px;
			padding:1px;
		}
		.box:focus{
			outline:none;
		}
		.mbox:focus{
			outline:none;
		}
		.online{
			color:wheat;
			font-size:10px;
		}
		.container-fluid{
			padding:0px;
		}
	</style>
<title>聊天室</title>
</head>
<body>
<div class="container-fluid">
	<div class="row-fluid">
		<div class="span12">
			<input class="box" type="text" oninput="coloregg();">
			<button class="btn btn-large btn-success sub" onclick="sends()" type="button">发送</button>
			<div class="online">正在获取在线人数……</div>
			<textarea class="mbox" type="text" readonly="true"></textarea>
		</div>
	</div>
</div>
	
	<br>
</body>
</html>