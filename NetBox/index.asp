<%@ Language="VBScript" %>
<!DOCTYPE html>
<html>
	<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">

	<head>
		<meta charset="utf-8">
		<title>忘不掉的那抹乡愁</title>
		<link rel="stylesheet" media="screen" href="style.css">
		<link rel="stylesheet" media="screen" href="form.css">
		<meta name="viewport" content="width=device-width, initial-scale=1.0">
		<meta name="description" content="This is a work for homework.">
		<meta name="robots" content="all">
		<script type="text/javascript" src="js/mymouse.js" id="mymouse"></script>
	</head>

	<body>
		<div class="page-wrapper">

			<section class="intro" id="zen-intro">
				<header role="banner">
					<h1>听听那冷雨</h1>
					<h2>惊蛰一过 春寒加剧</h2>
					<p><%Response.Write(now())%></p>
				</header>

				<div class="preamble" id="zen-preamble" role="article">
					<h3>五陵少年 · 歌</h3>
					<p>余光中（1928年10月21日—2017年12月14日），当代著名作家、诗人、学者、翻译家，出生于南京，祖籍福建永春。因母亲原籍为江苏武进，故也自称“江南人”。 </p>
					<p>余光中一生从事诗歌、散文、评论、翻译，自称为自己写作的“四度空间”，被誉为文坛的“璀璨五彩笔”。</p>
					<p>驰骋文坛逾半个世纪，涉猎广泛，被誉为“艺术上的多妻主义者”。其文学生涯悠远、辽阔、深沉，为当代诗坛健将、散文重镇、著名批评家、优秀翻译家。</p>
					<p>代表作有《白玉苦瓜》（诗集）、《记忆像铁轨一样长》（散文集）及《分水岭上：余光中评论文集》（评论集）等，其诗作如《乡愁》、《乡愁四韵》，散文如《听听那冷雨》、《我的四个假想敌》等，广泛收录于大陆及港台语文课本。 </p>
				</div>
			</section>

			<div class="main supporting" id="zen-supporting" role="main">
				<div class="explanation" id="zen-explanation" role="article">
					<h3>白玉苦瓜 · 隔水观音</h3>
					<p>2017年12月14日，余光中教授于台湾逝世，享年89岁。</p>
				</div>

				<div class="participation" id="zen-participation" role="article">
					<h3>日不落家 · 老人和大海</h3>
					<p>余光中生于中国江苏南京，父亲余超英，母亲孙秀君。但是母亲为江苏武进人，妻子为常州人，故又以江南人自命。抗日战争时在四川读中学，感情上亦自觉为蜀人。曾自言：“大陆（中国大陆）是母亲，台湾是妻子，香港是情人，欧洲是外遇。” </p>
				</div>

				<div class="benefits" id="zen-benefits" role="article">
					<h3>左手的缪思 · 逍遥游</h3>
					<p>其文学生涯悠远、辽阔、深沉，且兼有中国古典文学与外国现代文学之精神，创作手法新颖灵活，比喻奇特，描写精雕细刻，抒情细腻缠绵，一唱三叹，含蓄隽永，意味深长，韵律优美，节奏感强。他因此被尊为台湾诗坛祭酒。他的诗论视野开阔，富有开拓探索的犀利朝气；他强调作家的民族感和责任感，善于从语言的角度把握诗的品格和价值，自成一家。 </p>
				</div>

				<div class="i_say" role="article">
					<h3>不可儿戏 · 守夜人</h3>
					<p>余光中走了，在七十年的台湾文化史上，是一个时代的结束。从疼痛彻骨的迁徙流亡思乡，到意气风发的“希腊天空”的追寻，到回眸凝视决定拥抱枋寮的泥土，到最后在自己拥抱的泥土上又变成异乡人，余光中的一生就是一部跨世纪的疼痛文化史。</p>
				</div>

				<footer>
					<pre>
        	“这是最纵容最宽阔的床
            				让一颗心满足地睡去，满足地想”
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
									<a href="index.asp" class="design-name">听听那冷雨</a>
								</li>
								<li>
									<a href="article1.asp" class="design-name">等你，在雨中</a>
								</li>
								<li>
									<a href="article2.asp" class="design-name">布谷</a>
								</li>
								<li>
									<a href="article3.asp" class="design-name">风铃</a>
								</li>
								<li>
									<a href="article4.asp" class="design-name">乡愁</a>
								</li>
								<li>
									<a href="article5.asp" class="design-name">中秋</a>
								</li>
								<li>
									<a href="mailto:xxx@163.com?subject=The%20subject%20of%20the%20mail">给WO发送邮件</a>
								</li>
								<li>
									<br><br><br>
									<a href="http://www.dlou.edu.cn" onclick="window.external.addFavorite(this.href,this.title);return false;" title='大连海洋大学欢迎您' rel="sidebar">加入收藏</a>
								</li>
									<form method="post" class="basic-grey" action="admin.asp">
										<h1>LOG_IN
										<span>站长登录</span>
										</h1>
										<label>
											<span>名字 :</span>
											<input id="name" type="text" name="name" placeholder="Your Full Name" />
										</label>
										<label>
											<span>密码 :</span>
                                            <input id="pwd" type="text" name="password" placeholder="Your Full Password"/>
										</label>
										<label>
											<span>点下试试：</span>
											<input type="Submit" class="button" value="Submit" />
										</label>
									</form>
									<br><br><br><br>
									<!--Baidu搜索开始-->
									<form target="_blank" action="http://zhannei.baidu.com/cse/site" class="basic-grey">
									<h1>SEARCH</h1>
									<input type="text" name="q" size="30">
									<input type="submit" value="搜索" class="button">
									<input type="hidden" name="cc">
									</form>
									<!--Baidu搜索结束-->
							</ul>
						</nav>
					</div>

				</div>
			</aside>

		</div>
	</body>

</html>