<%@ Language="VBScript" %>
<!DOCTYPE html>
<html>
  <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<head>
    <meta charset="utf-8">
    <title>登录</title>
    <script type="text/javascript">
        function showLogin() {
            var loginDiv = document.getElementById("loginDiv");
            var zhezhao = document.getElementById("zhezhao");
            var clientx = document.documentElement.clientWidth;
            var clienty = document.documentElement.clientHeight;
            var l_margin = (clientx - parseInt(loginDiv.style.width)) / 2;
            var t_margin = (clienty - parseInt(loginDiv.style.height)-200) / 2
            loginDiv.style.left = l_margin + "px";
            loginDiv.style.top = t_margin +"px";
            loginDiv.style.display = "block";
            zhezhao.style.display = "block";
        }
        function hidLogin() {
            var loginDiv = document.getElementById("loginDiv");
            var zhezhao = document.getElementById("zhezhao");
            loginDiv.style.display = "none";
            zhezhao.style.display = "none";
        }
        function titleMove() {
            var moveable = true;
            var loginDiv = document.getElementById("loginDiv");
            //以下变量提前设置好                        var clientX = window.event.clientX;
            var clientY = window.event.clientY;
            var moveTop = parseInt(loginDiv.style.top);
            var moveLeft = parseInt(loginDiv.style.left);
            document.onmousemove = function MouseMove() {
                if (moveable) {
                    var y = moveTop + window.event.clientY - clientY;
                    var x = moveLeft + window.event.clientX - clientX;
                    if (x > 0 && y > 0) {
                        loginDiv.style.top = y + "px";
                        loginDiv.style.left = x + "px";
                    }
                }
            }
            document.onmouseup = function Mouseup() {
                moveable = false;
            }
        }
    </script>
</head>

<body>
<%
dim fs, admin, user, pwd

user = Request.Form("name")
pwd = Request.Form("password")
admin = "D:\DESK\NetBox\" & user & "-" & pwd 

set fs = Server.CreateObject("Scripting.FileSystemObject")
if fs.FileExists(admin)=true then
    Response.Redirect "about.asp"
else
    Response.write "名字密码无效,Sir,这不是你的地盘！" & user
end if
set fs = nothing
%>
</body>
</html>