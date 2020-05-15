<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml">
	<head>
		<meta charset="UTF-8" />
		<title>GML-Toolbox - 3D Viewer</title>
		<!--link rel="stylesheet" type="text/css" href="x3dom.css" /-->
		<!--link rel="stylesheet" type="text/css" href="http://www.x3dom.org/x3dom/release/x3dom.css" /-->
		<link rel="shortcut icon" href="favicon.ico" type="image/x-icon"></link>
		
		<link rel="stylesheet" href="style.css" type="text/css" />
		
		<!--link rel="stylesheet" type="text/css" href="x3dom.css" /-->
		
		<script type="text/javascript" src="x3dom.js"></script>
		
		<script type="text/javascript" src="X3D-Viewer.js"></script>
		
	</head>
	<body onload="init()" onkeyup="keyFunction(event)">
		<div id="shadow">
			<div id="TitleFrame">
				<div id="Title" class="margin">
					<h1>GML-Toolbox - 3D Viewer</h1> <h2>(<i>
#FileName
					</i>)</h2>
				</div>
			</div>
			
			<div id="LogoFrame">
				<div class="margin">
					<img id="Logo" src="kit_logo.png" alt="KIT logo" />
				</div>
			</div>
		</div>
		<!--X3D id="x3d_scene" width="500px" height="500px">
			<Scene>
				<Background DEF="bnd" transparency="0.0" skyColor="0 0 0"/>
				<Inline id="QuadratX3D" url="X3D_Spitzdach_Layer.x3d" />
			</Scene>
		</X3D-->
		<div id="x3dScene">
#X3D
		</div>
		<div id="openNav" onclick="showHideBar(document.getElementById('Navigation'))" onmouseover="this.style.cursor = 'pointer'" title="Show navigation bar (key left)">
			<div class="margin">
				►
			</div>
		</div>
		<div id="Navigation" class="bar" style="display: none">
			<ul id="Navigation_list">
				<!--layer></layer-->
				<li onclick="showHideMenu(this)" onmouseover="this.style.cursor = &apos;pointer&apos;" title="View"><img class="viewNav" src="kit_logo.png" alt="View" />
					<ul onmouseout="showHideMenu(this.parentNode)">
						<li onclick="element.runtime.resetView()" onmouseover="this.style.cursor = 'pointer'" title="Resets view to initial view">
							Reset view (R)
						</li>
						<li onclick="element.runtime.showAll()" onmouseover="this.style.cursor = 'pointer'" title="Zooms to extent">
							Show all (A)
						</li>
						<li onclick="element.runtime.showAll('posZ')" onmouseover="this.style.cursor = 'pointer'" title="Zooms to extent">
							Show back side
						</li>
						<li onclick="element.runtime.showAll('posX')" onmouseover="this.style.cursor = 'pointer'" title="Zooms to extent">
							Show left side
						</li>
						<li onclick="element.runtime.showAll('negX')" onmouseover="this.style.cursor = 'pointer'" title="Zooms to extent">
							Show right side
						</li>
						<li onclick="element.runtime.showAll('posY')" onmouseover="this.style.cursor = 'pointer'" title="Zooms to extent">
							Show bottom
						</li>
						<li onclick="element.runtime.showAll('negY')" onmouseover="this.style.cursor = 'pointer'" title="Zooms to extent">
							Show top
						</li>
					</ul>
				</li>
				<li onclick="showHideMenu(this)" onmouseover="this.style.cursor = &apos;pointer&apos;" title="Navigation mode"><img class="viewNav" src="kit_logo.png" alt="Navigation mode" />
					<ul onmouseout="showHideMenu(this.parentNode)">
						<li onclick="element.runtime.examine()" onmouseover="this.style.cursor = 'pointer'" title="Examine">
							Examine (E)
						</li>
						<li onclick="element.runtime.walk()" onmouseover="this.style.cursor = 'pointer'" title="Walk">
							Walk (W)
						</li>
						<li onclick="element.runtime.lookAt()" onmouseover="this.style.cursor = 'pointer'" title="Look at">
							Look at (L ? O)
						</li>
						<li onclick="element.runtime.lookAround()" onmouseover="this.style.cursor = 'pointer'" title="Look around">
							Look around (O ? L)
						</li>
						<li onclick="element.runtime.game()" onmouseover="this.style.cursor = 'pointer'" title="Game">
							Game (G)
						</li>
						<li onclick="element.runtime.helicopter()" onmouseover="this.style.cursor = 'pointer'" title="Helicopter">
							Helicopter (H)
						</li>
					</ul>
				</li>
				<li onclick="showHideBar(document.getElementById('Navigation'))" onmouseover="this.style.cursor = 'pointer'" title="Hide navigation bar (key left)">◄
				</li>
			</ul>
		</div>
		
		<div id="openLayer" onclick="showHideBar(document.getElementById('LayerBar'))" onmouseover="this.style.cursor = 'pointer'" title="Show layer bar (key right)">
			<div class="margin">
				◄
			</div>
		</div>
		
		<div id="LayerBar">
			<div id="Tab" class="bar">
				
				<table id="tabTable">
					<tr>
						<td id="layer0_Active" class="Tab" title="Shows all Objects inside the tree of the Scene (1)" onclick="setTab('layer0_Active','layer0_Content','#999999')" onmouseover="this.style.cursor = 'pointer'">Scene <br /> graph</td>
						<td id="layer1_Active" class="Tab" title="Shows all Feature types existing inside the Scene (2)" onclick="setTab('layer1_Active','layer1_Content','#999999')" onmouseover="this.style.cursor = 'pointer'">Feature <br /> type</td>
						<td id="layer2_Active" class="Tab" title="Choose to see Point, Line and / or Face geometry (3)" onclick="setTab('layer2_Active','layer2_Content','#999999')" onmouseover="this.style.cursor = 'pointer'">Display <br /> mode</td>
						<td id="layer3_Active" class="Tab" title="Settings (4)" onclick="setTab('layer3_Active','layer3_Content','#999999')" onmouseover="this.style.cursor = 'pointer'">Settings</td>
						<td id="layer4_Active" class="Tab" title="Hide layer bar (key right)" onclick="showHideBar(document.getElementById('LayerBar'))" onmouseover="this.style.cursor = 'pointer'">►</td>
					</tr>
				</table>
			</div>
				
			<div id="LayerFrameShadow" class="bar">
				<div id="LayerFrame">
					<div class="margin">
						<div id="layer0_Content" class="LayerContent">
#SceneGraph
						</div>
						
						
						<div id="layer1_Content" class="LayerContent">
#FeatureType
							<!--dl>
								<dt id="CityGML_Building_Active" class="LayerLink1"><input type="checkbox" id="CityGML_Building_Layer" class="Building" name="Layer" value="CityGML_Building" checked="checked" onchange="visibility(this)"/> Buildings </dt>
								<dt id="CityGML_Solid_Active" class="LayerLink1"><input type="checkbox" id="CityGML_Solid_Layer" class="Solid" name="Layer" value="CityGML_Solid" checked="checked" onchange="visibility(this)"/> Solids </dt>
								<dt id="CityGML_RoofSurface_Active" class="LayerLink1"><input type="checkbox" id="CityGML_RoofSurface_Layer" class="RoofSurface" name="Layer" value="CityGML_RoofSurface" checked="checked" onchange="visibility(this)"/> RoofSurfaces </dt>
								<dt id="CityGML_WallSurface_Active" class="LayerLink1"><input type="checkbox" id="CityGML_WallSurface_Layer" class="WallSurface" name="Layer" value="CityGML_WallSurface" checked="checked" onchange="visibility(this)"/> WallSurfaces </dt>
								<dt id="CityGML_GroundSurface_Active" class="LayerLink1"><input type="checkbox" id="CityGML_GroundSurface_Layer" class="GroundSurface" name="Layer" value="CityGML_GroundSurface" checked="checked" onchange="visibility(this)"/> GroundSurfaces </dt>
							</dl-->
						</div>
						
						<div id="layer2_Content" class="LayerContent">
							<dl>
								<dt id="Point_Active" class="LayerLink2"><input type="checkbox" id="Point_Layer" class="Point" name="Layer" value="Point" onchange="visibility(this)"/> Point </dt>
								<dt id="Line_Active" class="LayerLink2"><input type="checkbox" id="Line_Layer" class="Line" name="Layer" value="Line" onchange="visibility(this)"/> Line </dt>
								<dt id="Face_Active" class="LayerLink2"><input type="checkbox" id="Face_Layer" class="Face" name="Layer" value="Face" checked="checked" onchange="visibility(this)"/> Face </dt>
							</dl>
						</div>
						
						<div id="layer3_Content" class="LayerContent">
							TODO!!!
						</div>
					</div>
				</div>
			</div>
		</div>
		
		<div id="openInfo" onclick="showHideBar(document.getElementById('InfoFrame'))" onmouseover="this.style.cursor = 'pointer'" title="Show info bar (key down)">
			<div class="margin">
				▲
			</div>
		</div>
		
		<div id="InfoFrame" class="bar" style="display: block">
			<div id="InfoTab">
				<table id="pathTable">
					<tr id="pathTableRow">
						<th title="Hide info bar (key down)" class="PathTab" onclick="showHideBar(document.getElementById('InfoFrame'))" onmouseover="this.style.cursor = 'pointer'">▼</th>
					</tr>
				</table>
			</div>
			
			<div id="InfoContentShadow">
				<div id="InfoContentFrame">
					<div class="margin">
						<div id="InfoContent" class="InfoContent"></div>
					</div>
				</div>
			</div>
		</div>
		<div id="infoBubble"><div id="infoBubbleContent" class="margin"> </div></div>
		<script type="text/javascript">
			/***var e = document.getElementById("CityGML_RoofSurface_Bernharduskirche_Roof1");
			e.runtime.showAll();
			e.runtime.resetView();**
			
			var info, info_pane;
			info = x3dom.docs.getNodeTreeInfo();
			console.log(info);
			info_pane = document.getElementById("InfoContent");
			info_pane.innerHTML = "info";*/
			
			//element.runtime.debug(true); //runtime.showObject(document.getElementById("CityGML_RoofSurface_Bernharduskirche_Roof1"), "posZ");
		</script>
	</body>
</html>