<%@ Language="VBScript" %>
<!DOCTYPE html>
<html>
    <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">

    <head>
        <meta charset="utf-8">
        <title>关于这个站点</title>
        <link rel="stylesheet" media="screen" href="../style.css">
        <link rel="stylesheet" media="screen" href="form.css">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <meta name="description" content="This is a work for homework.">
        <meta name="robots" content="all">
        <script type="text/javascript" src="js/mymouse.js" id="mymouse"></script>
        <script type="text/javascript"> 
        <!-- 
        function MM_jumpMenu(targ,selObj,restore){ //v3.0 
        eval(targ+".location='"+selObj.options[selObj.selectedIndex].value+"'"); 
        if (restore) selObj.selectedIndex=0; 
        } 
        //--> 
        </script> 
    </head>

    <body>
        <div class="page-wrapper">

            <section class="intro" id="zen-intro">
                <header role="banner">
                    <h1>我在，而你已不再</h1>
                    <h2>念</h2>
                    <p><%Response.Write(now())%></p>
                </header>

                <div class="preamble" id="zen-preamble" role="article">
                    <h3>话</h3>
                    <p>
                        <br> 一直在读你
                        <br> 直到把自己也读成一首
                        <br> 白发苍苍的乡愁
                        <br> 
                        <br> 而我是幸福的
                        <br> 每年都会数次回归诺敏河
                        <br> 给父亲斟满滚烫的高粱烧
                        <br> 为母亲的梦掖严被角
                        <br> 与故乡黑崴子留张合影
                        <br> 
                        <br> 而您也并不孤独
                        <br> 长长的九十载，陪伴你的
                        <br> 有一祯揉皱的邮票
                        <br> 几张窄窄的船票
                        <br> 还有一个用拐杖
                        <br> 敲打命运的瞎子
                        <br> 
                        <br> 当你驾鹤西去
                        <br> 全世界都在读你
                        <br> 湿漉漉的句子带着颤音
                        <br> 抚摸着你发光的名字
                        <br> 
                        <br> 在这个难眠的冬夜
                        <br> 面对你瘦骨嶙峋的肖像
                        <br> 遥望穿不过薄薄的镜片
                        <br> 
                        <br> 我把画有雄鸡版图的地球仪
                        <br> 凑近你，凑近你的眼前 唇边
                        <br> 打开室内所有的灯，想让你一次
                        <br> 看个够，亲个够
                    </p>
                </div>
            </section>

            <div class="main supporting" id="zen-supporting" role="main">

                <div class="participation" id="zen-participation" role="article">
                    <h3>记</h3>
                    <p>
                        <br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; 这首诗，是网上一首悼念余光中老先生的诗文。摘下来，作为纪念。
                        <br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; 对于余光中老先生，一首诗文足以胜过千言万语。
                    </p>
                </div>

                <footer>
                    <pre>
            “可怜良月仙魂去
                           常使文人泪雨流”
            </pre>
                </footer>

            </div>

            <aside class="sidebar" role="complementary">
                <div class="wrapper">

                    <div class="design-selection" id="design-selection">
                        <h3 class="select">绝色</h3>
                        <nav role="navigation">
                            <ul>
                                <li>
                                    <form action="" method="get" class="basic-grey"> 
                                    <h1>GOTO</h1> 
                                    <select name="jumpMenu" id="jumpMenu"
                                     onchange="MM_jumpMenu('parent',this,0)"> 
                                    <option value="index.asp">听听那冷雨</option> 
                                    <option value="article1.asp">等你，在雨中</option>
                                    <option value="article2.asp">布谷</option>
                                     <option value="article3.asp">风铃</option>
                                     <option value="article4.asp">乡愁</option>
                                     <option value="article5.asp">中秋</option> 
                                    </select> 
                                    </form> 
                                </li>
                                <li>
                                    <!--Baidu搜索开始-->
                                    <form target="_blank" action="http://zhannei.baidu.com/cse/site" class="basic-grey">
                                    <h1>SEARCH</h1>
                                    <input type="text" name="q" size="30">
                                    <input type="submit" value="搜索" class="button">
                                    <input type="hidden" name="cc">
                                    </form>
                                    <!--Baidu搜索结束-->
                                </li>
                            </ul>
                        </nav>
                    </div>

                </div>
            </aside>

        </div>
    </body>

</html>