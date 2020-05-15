var objSelectedX3D;
var objSelectedLayer;
var objSelectedAppearance;
var objOldMaterialUSE;
var objZoomTo;

var element;

var debug = false;

function init(){
	setTab('layer0_Active','layer0_Content','#999999');
	element = document.getElementById("x3d");
	//console.log(getElementById);
	document.getElementById("CityGML_WallSurface_Bernharduskirche_Wall4_Layer").checked = true;
}

			
function toggleDebug() {
	if (debug) {
		element.runtime.debug(false);
		console.log("false");
	} else {
		element.runtime.debug(true);
		console.log("true");
	}
	debug = !debug
}




// expands/retract Layergroups (inspired by http://www.chathomepage.de/pagedout/javascript/flip.html)
function expand(strClassName){
	var strButtonID = strClassName + "_Button";
	var objButton = document.getElementById(strButtonID);
	
	var strDivExpandID = strClassName + "_Expand";
	var objDivExpand = document.getElementById(strDivExpandID);
	
	if(objDivExpand.style.display == "none"){
		objButton.innerHTML = "-";
		objDivExpand.style.display = "block";
	}else{
		objDivExpand.style.display = "none";
		objButton.innerHTML = "+";
	}
}


////// FUNCTIONS FOR SELECTION OF OBJECTS //////
//General function which calls all the other
function elementSelection(strElement){
	showMeta(strElement);
	//setBackgroundColorByClassName(document.getElementById(strElement + "_Active").getAttribute("class"), "")
	removeShadowByClassName(document.getElementById(strElement + "_Active").getAttribute("class"));
	setActive(strElement + "_Active", "#009988","shadow");
	//document.getElementById(strElement + "_Active").style.boxShadow = "0px 1px 2px 1px #000000";
	setTab(strElement + "_path","InfoContent","#E0E0E0");
	//selectX3DObject(strElement);
	var objSelect = document.getElementById("InfoSelect");
	if (document.getElementById("InfoSelect_Line") != null){
		objSelect.removeChild(document.getElementById("InfoSelect_Line"));
	}
	
	var boxVol     = document.getElementById(strElement)._x3domNode.getVolume();
	
    var min = new x3dom.fields.SFVec3f();
    var max = new x3dom.fields.SFVec3f();

    boxVol.getBounds(min, max);
        
    var objElemIndexedLineSet = document.createElement("IndexedLineSet");
    var objElemCoordinate = document.createElement("Coordinate");
    
    var objAttrId = document.createAttribute("id");
    var objAttrPoint = document.createAttribute("point");
	var objAttrCoordIndex = document.createAttribute("coordIndex");
	
	objAttrId.nodeValue = "InfoSelect_Line";
	
	strAttrPoint = min.x.toString() + " " + min.y.toString() + " " + max.z.toString() + "    ";						// coordIndex 0
	strAttrPoint = strAttrPoint + max.x.toString() + " " + min.y.toString() + " " + max.z.toString() + "    ";		// coordIndex 1
	strAttrPoint = strAttrPoint + max.x.toString() + " " + max.y.toString() + " " + max.z.toString() + "    ";		// coordIndex 2
	strAttrPoint = strAttrPoint + min.x.toString() + " " + max.y.toString() + " " + max.z.toString() + "    ";		// coordIndex 3
	strAttrPoint = strAttrPoint + max.x.toString() + " " + min.y.toString() + " " + min.z.toString() + "    ";		// coordIndex 4
	strAttrPoint = strAttrPoint + min.x.toString() + " " + min.y.toString() + " " + min.z.toString() + "    ";		// coordIndex 5
	strAttrPoint = strAttrPoint + min.x.toString() + " " + max.y.toString() + " " + min.z.toString() + "    ";		// coordIndex 6
	strAttrPoint = strAttrPoint + max.x.toString() + " " + max.y.toString() + " " + min.z.toString();				// coordIndex 7
	objAttrPoint.nodeValue = strAttrPoint;
	
	objAttrCoordIndex.nodeValue = "0 1 2 3 -1    1 4 7 2 -1    4 5 6 7 -1    5 0 3 6 -1    3 2 7 6 -1    5 4 1 0 -1";
	
	objElemIndexedLineSet.setAttributeNode(objAttrId);
	objElemIndexedLineSet.setAttributeNode(objAttrCoordIndex);
	objElemCoordinate.setAttributeNode(objAttrPoint);
	
	objElemIndexedLineSet.appendChild(objElemCoordinate);
	objSelect.appendChild(objElemIndexedLineSet);
}

// showing Metadatas of clicked object (Layer/x3d-Object)
function showMeta(strElement){
	var objElement = document.getElementById(strElement + "_Meta");
	var string;
	if(objElement != undefined){
		var arrChilds = objElement.childNodes;
		var boolRow = false;
		var j = 0;
		string = "<table>";
		
		for(var i = 0; i < arrChilds.length; i++){
			//alert(arrChilds[i]);
			if(arrChilds[i].innerHTML != undefined){
				j++;
				if((j-1)%3==0){
					string = string + "<tr>";
				}
				if(boolRow == false){
					string = string + "<th>" + arrChilds[i].name + ":</th>";
					string = string + "<td>" + arrChilds[i].innerHTML + "</td>";
				}
				else{
					string = string + "<th class='modulo'>" + arrChilds[i].name + ":</th>";
					string = string + "<td class='modulo'>" + arrChilds[i].innerHTML + "</td>";
				}
				
				if(j%3==0){
					string = string + "</tr>";
					if(boolRow == false){
						boolRow = true;
					}
					else{
						boolRow = false;
					}
				}
				
			}
		}
		if(j%3!=0){
			string = string + "</tr>";
		}
		string = string + "</table>";
	}
	else{
		console.log(strElement);
		string = "There is no information about the Object '" + document.getElementById(strElement).getAttribute("name") + "'.";
	}
	console.log(string);
	document.getElementById("InfoContent").innerHTML = string;
}

// highlighting selected Layer or Tab
function setActive(strElementId, strColor, strType){
	var objElement = document.getElementById(strElementId);
	if(strType == "background"){
		objElement.style.backgroundColor = strColor;
	}
	else if (strType == "shadow"){
		objElement.style.boxShadow = "0px 1px 2px 1px #000000";
	}
	else{
		console.log(TypeError(strType + " is wrong. You need one of the follow words: 'background','shadow'."));
	}
}


// removing background color of Tags
function setBackgroundColorByClassName(strClassName, strHexColor){
	var arrElements = document.getElementsByClassName(strClassName);
	for (var i = 0; i < arrElements.length; i++){	
		arrElements[i].style.backgroundColor = strHexColor;
	}
}

// reseting shadow color of Tags
function removeShadowByClassName(strClassName){
	var arrElements = document.getElementsByClassName(strClassName);
	for (var i = 0; i < arrElements.length; i++){	
		arrElements[i].style.boxShadow = "0px 0px 0px 0px #FFFFFF";
	}
}

// reseting z-index of Tags
function setZIndexByClassName(strClassName, intIndexValue){
	var arrElements = document.getElementsByClassName(strClassName);
	for (var i = 0; i < arrElements.length; i++){	
		arrElements[i].style.zIndex = intIndexValue;
	}
}

function selectX3DObject(strElement){
	var objElement = document.getElementById(strElement + "_Appearance");
	console.log(strElement);
	console.log(objElement);
	if(objElement.hasChildNodes()){
		objElement.setAttribute("DEF", "Select");
		//alert(objElement.getAttribute("DEF"));
		var strColor = objElement.childNodes[1].getAttribute("diffuseColor");
		objElement.childNodes[1].setAttribute("diffuseColor", "1 1 0");
		//alert(objElement.getAttribute("id"));
		
		var strClass = objElement.parentNode.getAttribute("class");
		var arrApp = document.getElementsByTagName("Appearance");
		var bool = false;
		for (var i = 0; i < arrApp.length; i++){
			if(arrApp[i].parentNode.getAttribute("class") == strClass){
				//if(bool == true){
					changeAppearance(arrApp[i], strClass + "_Appearance", strColor);
					break
				//}
				//bool = true;
			}
		}
	}else{
		changeAppearance(objElement, "Select", "1 1 0");
		
	}
}

function changeAppearance(objAppearance, strDEF, strColor){
	var strID = objAppearance.getAttribute("id");
	var objParent = objAppearance.parentNode;
	objParent.removeChild(objAppearance);
	objAppearance = document.createElement("Appearance");
	objParent.insertBefore(objAppearance, objParent.childNodes[1]);
	
	addAttribute(objAppearance, "DEF", strDEF);
	
	addAttribute(objAppearance, "id", strID);
	
	var objMat = document.createElement("Material");
	objAppearance.appendChild(objMat);
	
	addAttribute(objAppearance.childNodes[0], "diffuseColor", "1 0 0"); //funzt nicht
	
	//alert(strColor);
	objAppearance.childNodes[0].setAttribute("diffuseColor", strColor); //funzt sonst nicht, wenn nicht doppelt
	//alert(objAppearance.getAttribute("DEF"));
}

//adds a new Attribute
function addAttribute(objElement, strAttrName, strAttrValue){
	var attrDEF = document.createAttribute(strAttrName);
	attrDEF.nodeValue = strAttrValue;
	objElement.setAttributeNode(attrDEF);
}




function setTab(strTabId, strContentId, strColor){
	var objContent = document.getElementById(strContentId);
	var objTab = document.getElementById(strTabId);
	resetTab(objContent.getAttribute("class"));
	setZIndexByClassName(objTab.getAttribute("class"), 2)
	objContent.style.display = "block";
	//alert(objTab.style.backgroundColor);
	objTab.style.zIndex = 3;
	setBackgroundColorByClassName(objTab.getAttribute("class"), "");
	setActive(strTabId, strColor, "background");
	//alert(document.getElementById(strTab + "_Content").style.display);
}

function resetTab(strClassName){
	var arrLayerContents = document.getElementsByClassName(strClassName);
	for(var i = 0; i < arrLayerContents.length; i++){
		//document.getElementById(arrLayerContents[i].id).style.display = "none";
		if(arrLayerContents[i].style.display == "block"){
			arrLayerContents[i].style.display = "none";
			break;
		}
	}
	
}

function select (objSel) {
	var objSelect = document.getElementById("Select");
	
	var arrObjSelName = document.getElementsByName(objSel.getAttribute("name"));
	//var objSelectUseDef = document.getElementById("Select_Appearance").getFieldValue("DEF");
	console.log(arrObjSelName);
	
	//var objGeomClone;
	
	/*var appOld = document.createElement("Appearance");
	var attrIDOld = document.createAttribute("id");
	var attrUSEOld = document.createAttribute("USE");*/
	
	/*var appNew = document.createElement("Appearance");
	var attrIDNew = document.createAttribute("id");
	var attrUSENew = document.createAttribute("USE");*/
	
	if(objSel.getAttribute("id") != null){
		if (objSelectedX3D == undefined){
			if (objSel.nodeName == "Shape" || objSel.nodeName == "Group") {
				console.log("if");
				objSelectedX3D = objSel;
				objSelectedLayer = document.getElementById(objSelectedX3D.id + "_Link");
			}
			else {
				console.log("else");
				console.log(arrObjSelName.length);
				objSelectedLayer = objSel;
				//objSelectedLayer = document.getElementById(objSelectedX3D.id + "_Active");
				for (var i=0; i<arrObjSelName.length; i++) {
					if (arrObjSelName[i].nodeName == "Shape" || arrObjSelName[i].nodeName == "Group") {
						objSelectedX3D = arrObjSelName[i];
					}
				}
				
				console.log("WTF");
				console.log(objSelectedX3D);
				console.log("WTF");
			}
			
			
			/*objSelectedAppearance = document.getElementById(objSelectedX3D.id + "_Appearance");
			strOldMaterialUSE = objSelectedAppearance.getAttribute("USE");
			console.log("Old Material: " + objSelectedAppearance.getAttribute("USE"));
			objSelectedAppearance.setAttribute("USE", "Select_Appearance")
			console.log("New Material: " + objSelectedAppearance.getAttribute("USE"));
			
			objOldMaterialUSE = document.getElementById(objSelectedX3D.id + "_Appearance").getFieldValue("USE");
			
			document.getElementById(objSelectedX3D.id + "_Appearance").setFieldValue("USE", objSelectUseDef);*/
			console.log(objSelectedX3D);
			//objSelectedX3D._x3domNode.highlight(enable,"1 0 0");
			
			var arrAncestors = getAncestorPath(objSelectedX3D);
			var objTableRow = document.getElementById("pathTableRow");
			
			createAncestorPathTab(objTableRow, arrAncestors);
	/*
			objGeomClone = document.getElementById(objSelectedX3D.id + "_Poly").cloneNode(true);
			//objSelectedX3D.render = false;
			objGeomClone.id = "Select_Poly";
			
			objSelect.appendChild(objGeomClone);
	*/
			//console.log(objSelect.childNodes.getElementsByTagName("Appearance"));
			
			//var boxDOMNode = document.getElementById("mybox");        
    var boxVol     = objSelectedX3D._x3domNode.getVolume();

    var min = new x3dom.fields.SFVec3f();
    var max = new x3dom.fields.SFVec3f();

    boxVol.getBounds(min, max);
        
    var objElemIndexedFaceSet = document.createElement("IndexedFaceSet");
    var objElemCoordinate = document.createElement("Coordinate");
    
    var objAttrId = document.createAttribute("id");
    var objAttrPoint = document.createAttribute("point");
	var objAttrCoordIndex = document.createAttribute("coordIndex");
	
	objAttrId.nodeValue = "Select_Poly";
	
	strAttrPoint = min.x.toString() + " " + min.y.toString() + " " + max.z.toString() + "    ";						// coordIndex 0
	strAttrPoint = strAttrPoint + max.x.toString() + " " + min.y.toString() + " " + max.z.toString() + "    ";		// coordIndex 1
	strAttrPoint = strAttrPoint + max.x.toString() + " " + max.y.toString() + " " + max.z.toString() + "    ";		// coordIndex 2
	strAttrPoint = strAttrPoint + min.x.toString() + " " + max.y.toString() + " " + max.z.toString() + "    ";		// coordIndex 3
	strAttrPoint = strAttrPoint + max.x.toString() + " " + min.y.toString() + " " + min.z.toString() + "    ";		// coordIndex 4
	strAttrPoint = strAttrPoint + min.x.toString() + " " + min.y.toString() + " " + min.z.toString() + "    ";		// coordIndex 5
	strAttrPoint = strAttrPoint + min.x.toString() + " " + max.y.toString() + " " + min.z.toString() + "    ";		// coordIndex 6
	strAttrPoint = strAttrPoint + max.x.toString() + " " + max.y.toString() + " " + min.z.toString();				// coordIndex 7
	objAttrPoint.nodeValue = strAttrPoint;
	
	objAttrCoordIndex.nodeValue = "0 1 2 3 -1    1 4 7 2 -1    4 5 6 7 -1    5 0 3 6 -1    3 2 7 6 -1    5 4 1 0 -1";
	
	objElemIndexedFaceSet.setAttributeNode(objAttrId);
	objElemIndexedFaceSet.setAttributeNode(objAttrCoordIndex);
	objElemCoordinate.setAttributeNode(objAttrPoint);
	
	objElemIndexedFaceSet.appendChild(objElemCoordinate);
	objSelect.appendChild(objElemIndexedFaceSet);
			
			console.log(max);
			
			console.log("else");
			/*attrIDNew.nodeValue = objSelectedX3D.id + "_Appearance";
			attrUSENew.nodeValue = "Select_Appearance";
			appNew.setAttributeNode(attrIDNew);
			appNew.setAttributeNode(attrUSENew);
			
			obj = objSelectedX3D.replaceChild(appNew, document.getElementById(objSelectedX3D.id + "_Appearance"));
			
			console.log(obj);*/
			
			
			
			objSelectedLayer.parentNode.style.backgroundColor = "#009988";
			
			
		}
		else if (objSel.id == objSelectedLayer.id || objSel.id == objSelectedX3D.id || objSel.id == "Select") {
			//console.log("Old Material: " + objSelectedAppearance.getAttribute("USE"));
			//location.reload();
			objSelectedLayer.parentNode.style.backgroundColor = "";
			
			/*attrIDOld.nodeValue = objSelectedX3D.id + "_Appearance";
			attrUSEOld.nodeValue = strOldMaterialUSE;
			appOld.setAttributeNode(attrIDOld);
			appOld.setAttributeNode(attrUSEOld);
			obj = objSelectedX3D.replaceChild(appOld, document.getElementById(objSelectedX3D.id + "_Appearance"));
			
			console.log(obj);
			console.log(document.getElementById("Select_Appearance"));
			
			document.getElementById(objSelectedX3D.id + "_Appearance").setFieldValue("USE", objOldMaterialUSE);*/
			
			objSelect.removeChild(document.getElementById("Select_Poly"));
			
			var objInfoSelect = document.getElementById("InfoSelect");
			objInfoSelect.removeChild(document.getElementById("InfoSelect_Line"));
			
			var objTableRow = document.getElementById("pathTableRow");
			
			removeShadowByClassName("LayerLink0");
			document.getElementById("InfoContent").innerHTML = "";
			
			clearAncestorPathTab(objTableRow);
			clearElementsByClassName("ChildrenList")
			
			objSelectedLayer = undefined;
			objSelectedX3D = undefined;
			//objSelectedAppearance.setAttribute("USE", strOldMaterialUSE);
			//console.log("New Material: " + objSelectedAppearance.getAttribute("USE"));
			//objSelectedAppearance = undefined;
			//strOldMaterialUSE = "";
			
			
		}
		else {
			objSelectedLayer.parentNode.style.backgroundColor = "";
			
			/*attrIDOld.nodeValue = objSelectedX3D.id + "_Appearance";
			attrUSEOld.nodeValue = strOldMaterialUSE;
			appOld.setAttributeNode(attrIDOld);
			appOld.setAttributeNode(attrUSEOld);
			objSelectedX3D.replaceChild(appOld, document.getElementById(objSelectedX3D.id + "_Appearance"));
			
			console.log(document.getElementById(objSelectedX3D.id + "_Appearance"));*/
			
			//document.getElementById(objSelectedX3D.id + "_Appearance").setFieldValue("USE", objOldMaterialUSE);
			
			if (objSel.getAttribute("name") == objSel.getAttribute("id")) {
				objSelectedX3D = objSel;
				objSelectedLayer = document.getElementById(objSelectedX3D.id + "_Link");
			}
			else {
				objSelectedLayer = objSel;
				for (var i=0; i<arrObjSelName.length; i++) {
					if (arrObjSelName[i].nodeName == "Shape" || arrObjSelName[i].nodeName == "Group") {
						objSelectedX3D = arrObjSelName[i];
					}
				}
				//objSelectedX3D = document.getElementById(objSelectedLayer.getAttribute("name"));
			}
			//console.log("Old Material: " + objSelectedAppearance.getAttribute("USE"));
			/*objSelectedAppearance.removeAttribute("USE");
			var objUSEAttribute = document.createAttribute("USE");
			objUSEAttribute.nodeValue = strOldMaterialUSE;
			objSelectedAppearance.setAttributeNode(objUSEAttribute);*/
			
			/*strOldMaterialUSE = document.getElementById(objSelectedX3D.id + "_Appearance").getAttribute("USE");
			
			attrIDNew.nodeValue = objSelectedX3D.id + "_Appearance";
			attrUSENew.nodeValue = "Select_Appearance";
			appNew.setAttributeNode(attrIDNew);
			appNew.setAttributeNode(attrUSENew);
			
			objSelectedX3D.replaceChild(appNew, document.getElementById(objSelectedX3D.id + "_Appearance"));
			
			console.log(document.getElementById("Select_Appearance"));
			
			
			objOldMaterialUSE = document.getElementById(objSelectedX3D.id + "_Appearance").getFieldValue("USE");
			
			document.getElementById(objSelectedX3D.id + "_Appearance").setFieldValue("USE", objSelectUseDef);*/
			
			var arrAncestors = getAncestorPath(objSelectedX3D);
			var objTableRow = document.getElementById("pathTableRow");
			
			clearAncestorPathTab(objTableRow);
			clearElementsByClassName("ChildrenList");
			
			createAncestorPathTab(objTableRow, arrAncestors);
			
			//document.getElementById("Select_Poly").setAttribute("USE", objSelectedX3D.id + "_Poly");
			objSelect.removeChild(document.getElementById("Select_Poly"));
	/*		objGeomClone = document.getElementById(objSelectedX3D.id + "_Poly").cloneNode(true);
			//objSelectedX3D.render = false;
			objGeomClone.id = "Select_Poly";
			
			objSelect.appendChild(objGeomClone);
	*/		
	var boxVol     = objSelectedX3D._x3domNode.getVolume();

    var min = new x3dom.fields.SFVec3f();
    var max = new x3dom.fields.SFVec3f();

    boxVol.getBounds(min, max);
        
    var objElemIndexedFaceSet = document.createElement("IndexedFaceSet");
    var objElemCoordinate = document.createElement("Coordinate");
    
    var objAttrId = document.createAttribute("id");
    var objAttrPoint = document.createAttribute("point");
	var objAttrCoordIndex = document.createAttribute("coordIndex");
	
	objAttrId.nodeValue = "Select_Poly";
	
	strAttrPoint = min.x.toString() + " " + min.y.toString() + " " + max.z.toString() + "    ";						// coordIndex 0
	strAttrPoint = strAttrPoint + max.x.toString() + " " + min.y.toString() + " " + max.z.toString() + "    ";		// coordIndex 1
	strAttrPoint = strAttrPoint + max.x.toString() + " " + max.y.toString() + " " + max.z.toString() + "    ";		// coordIndex 2
	strAttrPoint = strAttrPoint + min.x.toString() + " " + max.y.toString() + " " + max.z.toString() + "    ";		// coordIndex 3
	strAttrPoint = strAttrPoint + max.x.toString() + " " + min.y.toString() + " " + min.z.toString() + "    ";		// coordIndex 4
	strAttrPoint = strAttrPoint + min.x.toString() + " " + min.y.toString() + " " + min.z.toString() + "    ";		// coordIndex 5
	strAttrPoint = strAttrPoint + min.x.toString() + " " + max.y.toString() + " " + min.z.toString() + "    ";		// coordIndex 6
	strAttrPoint = strAttrPoint + max.x.toString() + " " + max.y.toString() + " " + min.z.toString();				// coordIndex 7
	objAttrPoint.nodeValue = strAttrPoint;
	
	objAttrCoordIndex.nodeValue = "0 1 2 3 -1    1 4 7 2 -1    4 5 6 7 -1    5 0 3 6 -1    3 2 7 6 -1    5 4 1 0 -1";
	
	objElemIndexedFaceSet.setAttributeNode(objAttrId);
	objElemIndexedFaceSet.setAttributeNode(objAttrCoordIndex);
	objElemCoordinate.setAttributeNode(objAttrPoint);
	
	objElemIndexedFaceSet.appendChild(objElemCoordinate);
	objSelect.appendChild(objElemIndexedFaceSet);
			
			//objSelectedAppearance.setAttribute("USE", strOldMaterialUSE);
			//objSelectedAppearance = document.getElementById(objSelectedX3D.id + "_Appearance");
			
			
			
			//objSelectedAppearance.setAttribute("USE", "Select_Appearance");
			/*objSelectedAppearance.removeAttribute("USE");
			objUSEAttribute = document.createAttribute("USE");
			objUSEAttribute.nodeValue = "Select_Appearance";
			objSelectedAppearance.setAttributeNode(objUSEAttribute);*/
			
			//console.log("New Material: " + objSelectedAppearance.getAttribute("USE"));
			objSelectedLayer.parentNode.style.backgroundColor = "#009988";
		}
	}else{}
}

function zoomToObject(){
	//console.log(objZoomTo);
	var selObj = document.getElementById(objZoomTo.getAttribute("name"));
	//console.log(event.target.parentNode.parentNode.getAttribute("name"));
	element.runtime.showObject(selObj, 'negY');
}

function showOnlyObject(){
	//console.log(objZoomTo);
	var selObj = document.getElementById(objZoomTo.getAttribute("name"));
	//console.log(event.target.parentNode.parentNode.getAttribute("name"));
	
	var arrShapes = document.getElementsByTagName("Shape");
	
	for (var i=0; i < arrShapes.length; i++) {
		
		if((arrShapes[i].getAttribute("name") != selObj.getAttribute("name")) && (document.getElementById(arrShapes[i].id + "_Layer") != undefined)){
			document.getElementById(arrShapes[i].id + "_Layer").checked = false;
			visibility(document.getElementById(arrShapes[i].id + "_Layer"));
			console.log("hola");
			//arrShapes[i].setAttribute("render", "false");
			/*if(document.getElementById(arrShapes[i].id + "_Layer") != undefined){
				
				//visibility(arrShapes[i].getAttribute("name"));
				//console.log(arrShapes[i]);
			}*/
		}
	};
	
	element.runtime.showObject(selObj, 'negY');
}





// (un)rendering x3d objects
function visibility(objCheckbox){
	console.log(objCheckbox);
	var strTab = objCheckbox.parentNode.getAttribute("class");
	//alert(strLayerID);
	var objElement;
	var arrChildShapes
	switch(strTab){
		case "LayerLink0":
			objElement = document.getElementById(objCheckbox.value);
			var bGroup = false;
			/*if (document.getElementsByClassName(objCheckbox.value).length != 0){
				var arrClassLayer = document.getElementsByClassName(objCheckbox.value);
				
			}else{}*/
			
			//console.log(objElement.nodeName);
			
			if (objElement.nodeName != "Shape"){
				arrChildShapes = objElement.getElementsByTagName("Shape");
				bGroup = true;
			}
			
			if (objCheckbox.checked){
				
				if (bGroup == true){
					for (var i = 0; i < arrChildShapes.length; i++) {
						if(document.getElementById(arrChildShapes[i].id + "_Layer") != undefined){
							document.getElementById(arrChildShapes[i].id + "_Layer").checked = true;
						}
						arrChildShapes[i].setAttribute("render", true);
						document.getElementById(arrChildShapes[i].parentNode.getAttribute("id") + "_Layer").checked = true;
					}
				}else{
					objElement.setAttribute("render", true);
				}
			}else{
				
				if (bGroup == true){
					for (var i = 0; i < arrChildShapes.length; i++) {
						if(document.getElementById(arrChildShapes[i].id + "_Layer") != undefined){
							document.getElementById(arrChildShapes[i].id + "_Layer").checked = false;
						}
						arrChildShapes[i].setAttribute("render", false);
						document.getElementById(arrChildShapes[i].parentNode.getAttribute("id") + "_Layer").checked = false;
						//console.log(i);
					}
				}else{
					objElement.setAttribute("render", false);
				}
			}
			
			break;
		case "LayerLink1":
			objElement = document.getElementsByClassName(objCheckbox.value);
			
			for (var i=0; i < objElement.length; i++) {
			 	arrChildShapes = objElement[i].getElementsByTagName("Shape");
			 	console.log(objElement[i]);
			 	
				if (objCheckbox.checked){
					for (var j = 0; j < arrChildShapes.length; j++) {
						if(document.getElementById(arrChildShapes[j].id + "_Layer") != undefined){
							document.getElementById(arrChildShapes[j].id + "_Layer").checked = true;
						}
						arrChildShapes[j].setAttribute("render", true);
						document.getElementById(arrChildShapes[j].parentNode.getAttribute("id") + "_Layer").checked = true;
					}
				}else{
					for (var j = 0; j < arrChildShapes.length; j++) {
						if(document.getElementById(arrChildShapes[j].id + "_Layer") != undefined){
							document.getElementById(arrChildShapes[j].id + "_Layer").checked = false;
						}
						arrChildShapes[j].setAttribute("render", false);
						document.getElementById(arrChildShapes[j].parentNode.getAttribute("id") + "_Layer").checked = false;
					}
				}
			
			}
			break;
		case "LayerLink2":
			objElement = element.getElementsByClassName(objCheckbox.value);
			console.log(objElement);
			for (var i = 0; i < objElement.length; i++) {
				if (objCheckbox.checked) {
					objElement[i].setAttribute("whichChoice", 0);
				}else{
					objElement[i].setAttribute("whichChoice", -1);
				}
			}
			break;
	}
}

function showHideMenu(objList){
	if (objList.getElementsByTagName('ul')[0].style.display == 'block'){
		objList.getElementsByTagName('ul')[0].style.display = 'none';
	}else{
		objList.getElementsByTagName('ul')[0].style.display = 'block';
	}
}

function showHideBar(objBar){
	var objRectBar;
	var intWidthBar, intHeightBar, intLeftBar, intRightBar;
	var objX3dScene = document.getElementById("x3dScene");
	var objRectX3d = objX3dScene.getBoundingClientRect();
	var intWidthX3d = objRectX3d.width;
	var intHeightX3d = objRectX3d.height;
	
	var intLeftX3d = objRectX3d.left;
	var intRightX3d = objRectX3d.right;
	
	if(objBar.style.display == "block"){
		objRectBar = objBar.getBoundingClientRect();
		intWidthBar = objRectBar.width;
		intHeightBar = objRectBar.height;
		intLeftBar = objRectBar.left;
		intRightBar = objRectBar.right;
		
		console.log(objRectBar);
		
		if (intLeftBar == 0) {
			if(intWidthBar < intHeightBar){
				
				console.log("Nav hide");
				objX3dScene.style.left = "0px";
				
				var intWidth = intWidthX3d + intWidthBar;
				var fWidthPercent = pixel2Percent(intWidth, "width", window.innerWidth);
				var strWidthPercent = fWidthPercent.toString() + "%";
				console.log(strWidthPercent);
				objX3dScene.style.width = strWidthPercent;
				console.log(objX3dScene.style.width);
				
				objBar.style.display = "none";
			}else{
				console.log("Info hide");
				
				var objNavBar = document.getElementById("Navigation");
				var objRectNavBar = objNavBar.getBoundingClientRect();
				var intHeightNavBar = objRectNavBar.height;
				
				var objTitleBar = document.getElementById("shadow");
				var objRectTitleBar = objTitleBar.getBoundingClientRect();
				var intHeightTitleBar = objRectTitleBar.height;
				
				var intHeight = intHeightX3d + intHeightBar;
				
				console.log("  X3D height: "+intHeightX3d);
				console.log("+ Bar height: "+intHeightBar);
				console.log("+ Title height: "+intHeightTitleBar);
				var intTotal = intHeightX3d + intHeightBar + intHeightTitleBar;
				console.log("= Total height: "+intTotal);
				console.log("Total round: " + Math.floor(intTotal));
				
				var fHeightPercent = pixel2Percent(intHeight, "height", intTotal);
				var strHeightPercent = fHeightPercent.toString() + "%";
				console.log(strHeightPercent);
				objX3dScene.style.height = strHeightPercent;
				objNavBar.style.height = strHeightPercent;
				
				console.log(objX3dScene.style.height);
				
				objBar.style.display = "none";
			}
		}else{
			if(intWidthBar < intHeightBar){
				
				console.log("Layer hide");
				var boolInfoHide = false;
				
				var objInfoBar = document.getElementById("InfoFrame");
				if(objInfoBar.style.display == "none"){
					objInfoBar.style.display = "block";
					boolInfoHide = true;
				}
				var objRectInfoBar = objInfoBar.getBoundingClientRect();
				var intWidthInfoBar = objRectInfoBar.width;
				
				var intWidth = intWidthX3d + intWidthBar;
				var fWidthPercent = pixel2Percent(intWidth, "width", window.innerWidth);
				var strWidthPercent = fWidthPercent.toString() + "%";
				console.log(strWidthPercent);
				objX3dScene.style.width = strWidthPercent;
				
				var intInfoWidth = intWidthInfoBar + intWidthBar;
				var fInfoWidthPercent = pixel2Percent(intInfoWidth, "width", window.innerWidth);
				var strInfoWidthPercent = fInfoWidthPercent.toString() + "%";				
				objInfoBar.style.width = strInfoWidthPercent;
				if(boolInfoHide == true){
					objInfoBar.style.display = "none";
				}
				console.log(objX3dScene.style.width);
				
				objBar.style.display = "none";
			}/*else{
				console.log("Info hide");
				
				var intHeight = intHeightX3d + intHeightBar;
				var strHeightPx = intHeight.toString() + "px";
				console.log(strHeightPx);
				objX3dScene.style.height = strHeightPx;
				console.log(objX3dScene.style.height);
				
				objBar.style.display = "none";
			}*/
		}
	}else{
		objBar.style.display = "block";
		
		objRectBar = objBar.getBoundingClientRect();
		intWidthBar = objRectBar.width;
		intHeightBar = objRectBar.height;
		intLeftBar = objRectBar.left;
		intRightBar = objRectBar.right;
		
		console.log(objRectBar);
		
		if (intLeftBar == 0) {
			if(intWidthBar < intHeightBar){
				
				console.log("Nav show");
				var strLeftPx = intWidthBar + "px";
				
				objX3dScene.style.left = strLeftPx;
				
				var intWidth = intWidthX3d - intWidthBar;
				var fWidthPercent = pixel2Percent(intWidth, "width", window.innerWidth);
				var strWidthPercent = fWidthPercent.toString() + "%";
				console.log(strWidthPercent);
				objX3dScene.style.width = strWidthPercent;
				console.log(objX3dScene.style.width);
			}else{
				console.log("Info show");
				
				var objNavBar = document.getElementById("Navigation");
				var objRectNavBar = objNavBar.getBoundingClientRect();
				var intHeightNavBar = objRectNavBar.height;
				
				var objTitleBar = document.getElementById("shadow");
				var objRectTitleBar = objTitleBar.getBoundingClientRect();
				var intHeightTitleBar = objRectTitleBar.height;
				var intTotal = intHeightX3d + intHeightTitleBar;
				
				var intHeight = intHeightX3d - intHeightBar;
				var fHeightPercent = pixel2Percent(intHeight, "height", intTotal);
				var strHeightPercent = fHeightPercent.toString() + "%";
				console.log(strHeightPercent);
				objX3dScene.style.height = strHeightPercent;
				objNavBar.style.height = strHeightPercent;
				
				console.log(objX3dScene.style.height);
			}
		}else{
			if(intWidthBar < intHeightBar){
				
				console.log("Layer show");
				
				var objInfoBar = document.getElementById("InfoFrame");
				if(objInfoBar.style.display == "none"){
					objInfoBar.style.display = "block";
					boolInfoHide = true;
				}
				var objRectInfoBar = objInfoBar.getBoundingClientRect();
				var intWidthInfoBar = objRectInfoBar.width;
				
				var intWidth = intWidthX3d - intWidthBar;
				var fWidthPercent = pixel2Percent(intWidth, "width");
				var strWidthPercent = fWidthPercent.toString() + "%";
				console.log(strWidthPercent);
				objX3dScene.style.width = strWidthPercent;
				
				
				var intInfoWidth = intWidthInfoBar - intWidthBar;
				var fInfoWidthPercent = pixel2Percent(intInfoWidth, "width", window.innerWidth);
				var strInfoWidthPercent = fInfoWidthPercent.toString() + "%";
				objInfoBar.style.width = strInfoWidthPercent;
				if(boolInfoHide == true){
					objInfoBar.style.display = "none";
				}
			}/*else{
				console.log("Info hide");
				
				var intHeight = intHeightX3d + intHeightBar;
				var strHeightPx = intHeight.toString() + "px";
				console.log(strHeightPx);
				objX3dScene.style.height = strHeightPx;
				console.log(objX3dScene.style.height);
				
				objBar.style.display = "none";
			}*/
		}
	}
	
}

function pixel2Percent (fPixelValue, strDirection, fTotalValue) {
  	var fPercentValue;
  	if(strDirection == "height"){
  		fPercentValue = (100 / fTotalValue) * fPixelValue;
  		console.log("Inner height: "+window.innerHeight);
  		console.log("Outer height: "+window.outerHeight);
				
  		return fPercentValue;
  	}
  	else if (strDirection == "width"){
  		fPercentValue = (100 / window.innerWidth) * fPixelValue;
  		return fPercentValue;
  	}
  	else {
  		console.log("Error: '" + strDirection + "' is not correct. You have to write 'height' or 'width' instead.");
  	}
}

function getInfo (obj, event){
	var objInfoBubbleContent = document.getElementById('infoBubbleContent');
	var objInfoBubble = document.getElementById('infoBubble');
	objInfoBubble.style.display = "block";
	objInfoBubbleContent.innerHTML = "<table><tr><th>Name:</th><td>" + document.getElementsByName("X3D:LabelName_" + obj.id)[0].childNodes[0].textContent + "</td></tr><tr><th>Feature<br />type:</th><td>" + obj.getAttribute("class") + "</td></tr></table>";
	var arrCoord = event.hitPnt;
	console.log(arrCoord[0]);
	var arrPageCoord = element.runtime.calcPagePos(arrCoord[0], arrCoord[1], arrCoord[2]);
	objInfoBubble.style.left = arrPageCoord[0].toString() + "px";
	
	objInfoBubble.style.top = arrPageCoord[1].toString() + "px";
	console.log(objInfoBubble.style.left);
	
}

function deleteInfo (event){
	var objInfoBubbleContent = document.getElementById('infoBubbleContent');
	var objInfoBubble = document.getElementById('infoBubble');
	objInfoBubbleContent.innerHTML = "";
	objInfoBubble.style.display = "none";
}

function keyFunction(event){
	var keyCode = event.keyCode;
	if(keyCode == 27){
		/*var e = document.createEvent("HTMLEvents");
		e.initEvent("click", true, false);
		e.target = document.getElementById(objSelectedX3D.id);*/
		select(objSelectedX3D);
		console.log("'ESC' was clicked");
	}
	else if(keyCode == 49 || keyCode == 97){
		setTab('layer0_Active','layer0_Content','#999999');
	}
	else if (keyCode == 50 || keyCode == 98){
		setTab('layer1_Active','layer1_Content','#999999');
	}
	else if (keyCode == 51 || keyCode == 99){
		setTab('layer2_Active','layer2_Content','#999999');
	}
	else if (keyCode == 52 || keyCode == 100){
		setTab('layer3_Active','layer3_Content','#999999');
	}
	else if (keyCode == 37){
		showHideBar(document.getElementById('Navigation'));
	}
	else if (keyCode == 39){
		showHideBar(document.getElementById('LayerBar'));
	}
	else if (keyCode == 40){
		showHideBar(document.getElementById('InfoFrame'));
	}
}

// returns an array with the Group ancestor of an X3D object
function getAncestorPath (objX3D){
	var arrParents = [];
	console.log(objX3D);
	var objChild = objX3D;
	arrParents.push(objChild);
	//console.log(arrParents[0]);
	while (objChild.tagName != "Scene") {
		if (objChild.parentNode.tagName == "Group"){
			arrParents.unshift(objChild.parentNode);
			//console.log(arrParents[0]);
		}
		objChild = objChild.parentNode;
	}
	
	return arrParents;
}


function createAncestorPathTab (objNewParent, arrAncestorPathObjects){
	for(var i=0; i<arrAncestorPathObjects.length; i++){
		
		var objElemTd = document.createElement("td");
		var objAttrId = document.createAttribute("id");
		var objAttrOC = document.createAttribute("onclick");
		var objAttrOMOv = document.createAttribute("onmouseover");
		var objAttrClass = document.createAttribute("class");
		
		var arrObjChilds = arrAncestorPathObjects[i].childNodes;
		
		var arrObjGeomChilds = [];
		for (var j=0; j<arrObjChilds.length; j++) {
			console.log(arrObjChilds[j]);
			console.log(j);
			if ((arrObjChilds[j].tagName == "Group") || (arrObjChilds[j].tagName == "Shape" && arrObjChilds[j].id != undefined)) {
				arrObjGeomChilds.push(arrObjChilds[j]);
				console.log("hola");
			}
		}
		
		//console.log(arrObjSwitchChilds);
		objAttrId.nodeValue = arrAncestorPathObjects[i].id + "_path";
		objAttrOC.nodeValue = "elementSelection('" + arrAncestorPathObjects[i].id + "')";
		objAttrOMOv.nodeValue = "this.style.cursor = 'pointer'";
		objAttrClass.nodeValue = "PathTab";
		
		objElemTd.setAttributeNode(objAttrId);
		objElemTd.setAttributeNode(objAttrOC);
		objElemTd.setAttributeNode(objAttrOMOv);
		objElemTd.setAttributeNode(objAttrClass);
		
		var objTextNode = document.createTextNode(document.getElementsByName("X3D:LabelName_" + arrAncestorPathObjects[i].id)[0].childNodes[0].textContent);
		objElemTd.appendChild(objTextNode);
		objNewParent.appendChild(objElemTd);
		
		console.log(arrObjGeomChilds);
		console.log("Children");
		console.log(arrObjGeomChilds.length != 0);
		if (arrObjGeomChilds.length != 0) {
			
			var objElemChildrenTabTd = document.createElement("td");
			var objElemChildrenListDiv = document.createElement("div");
			var objElemChildrenListMarginDiv = document.createElement("div");
			var objElemChildrenListUl = document.createElement("ul");
			
			var objAttrChildrenTabId = document.createAttribute("id");
			var objAttrChildrenListId = document.createAttribute("id");
			
			var objAttrChildrenTabOC = document.createAttribute("onclick");
			
			var objAttrChildrenTabOMOv = document.createAttribute("onmouseover");
			
			var objAttrChildrenListOMOu = document.createAttribute("onmouseout");
			
			var objAttrChildrenTabClass = document.createAttribute("class");
			var objAttrChildrenListClass = document.createAttribute("class");
			var objAttrChildrenListMarginClass = document.createAttribute("class");
			
			
			objAttrChildrenTabId.nodeValue = arrAncestorPathObjects[i].id + "_Children_Tab";
			objAttrChildrenListId.nodeValue = arrAncestorPathObjects[i].id + "_Children_List";
			
			objAttrChildrenTabOC.nodeValue = "showHideHorizontalMenu(this, document.getElementById('" + arrAncestorPathObjects[i].id + "_Children_List'))";
			
			objAttrChildrenTabOMOv.nodeValue = "this.style.cursor = 'pointer'";
			
			objAttrChildrenListOMOu.nodeValue = "showHideHorizontalMenu(document.getElementById('" + arrAncestorPathObjects[i].id + "_Children_Tab'), this)";
			
			objAttrChildrenTabClass.nodeValue = "PathTab";
			objAttrChildrenListClass.nodeValue = "ChildrenList";
			objAttrChildrenListMarginClass.nodeValue = "margin";
			
			
			objElemChildrenTabTd.setAttributeNode(objAttrChildrenTabId);
			objElemChildrenListDiv.setAttributeNode(objAttrChildrenListId);
			
			objElemChildrenTabTd.setAttributeNode(objAttrChildrenTabOC);
			
			objElemChildrenTabTd.setAttributeNode(objAttrChildrenTabOMOv);
			
			objElemChildrenListDiv.setAttributeNode(objAttrChildrenListOMOu);
			
			objElemChildrenTabTd.setAttributeNode(objAttrChildrenTabClass);
			objElemChildrenListDiv.setAttributeNode(objAttrChildrenListClass);
			objElemChildrenListMarginDiv.setAttributeNode(objAttrChildrenListMarginClass);
			
			
			var objChildrenTabTextNode = document.createTextNode("►");
			
			
			objElemChildrenTabTd.appendChild(objChildrenTabTextNode);
			objNewParent.appendChild(objElemChildrenTabTd);
			objElemChildrenListMarginDiv.appendChild(objElemChildrenListUl);
			objElemChildrenListDiv.appendChild(objElemChildrenListMarginDiv);
			
			for (var j=0; j<arrObjGeomChilds.length; j++) {
				var objElemChildrenListLi = document.createElement("li");
				
				var objAttrChildrenListLiId = document.createAttribute("id");
				var objAttrChildrenListLiName = document.createAttribute("name");
				var objAttrChildrenListLiOC = document.createAttribute("onclick");
				var objAttrChildrenListLiOMOv = document.createAttribute("onmouseover");
				var objAttrChildrenListLiClass = document.createAttribute("class");
				
				objAttrChildrenListLiId.nodeValue = arrAncestorPathObjects[i].id + "_Children_List_Active";
				objAttrChildrenListLiName.nodeValue = arrAncestorPathObjects[i].getAttribute("name");
				objAttrChildrenListLiOC.nodeValue = "select(document.getElementById('" + arrObjGeomChilds[j].id + "'))";
				objAttrChildrenListLiOMOv.nodeValue = "this.style.cursor = 'pointer'";
				objAttrChildrenListLiClass.nodeValue = "ChildrenListLi";
				
				objElemChildrenListLi.setAttributeNode(objAttrChildrenListLiId);
				objElemChildrenListLi.setAttributeNode(objAttrChildrenListLiName);
				objElemChildrenListLi.setAttributeNode(objAttrChildrenListLiOC);
				objElemChildrenListLi.setAttributeNode(objAttrChildrenListLiOMOv);
				objElemChildrenListLi.setAttributeNode(objAttrChildrenListLiClass);
				
				var objChildrenListLiTextNode = document.createTextNode(arrObjGeomChilds[j].getAttribute("name"));
				
				objElemChildrenListLi.appendChild(objChildrenListLiTextNode);
				objElemChildrenListUl.appendChild(objElemChildrenListLi);
			}
			
			//objElemChildrenListDiv.style.zIndex = 4;
			document.getElementsByTagName("body")[0].appendChild(objElemChildrenListDiv);
			
		}
	}
	console.log(arrAncestorPathObjects[arrAncestorPathObjects.length-1]);
	elementSelection(arrAncestorPathObjects[arrAncestorPathObjects.length-1].id);
	console.log("hola");
}

function clearAncestorPathTab (objOldParent){
	var arrTd = objOldParent.getElementsByTagName("td");
	console.log(arrTd.length);
	
	while(arrTd.length != 0){
		console.log(arrTd.length);
		var objRemoved = objOldParent.removeChild(arrTd[arrTd.length-1]);
		console.log("removed:");
		console.log(objRemoved);
	}
	/*for(var i=0; i<arrTd.length; i++){
		console.log(i);
		var objRemoved = objOldParent.removeChild(arrTd[i]);
		console.log("removed:");
		console.log(objRemoved);
		console.log(arrTd.length);
	}*/
}

function clearElementsByClassName (strClassName){
	var arrClass = document.getElementsByClassName(strClassName);
	
	while(arrClass.length != 0){
		//console.log(arrClass.length);
		var objRemoved = arrClass[arrClass.length-1].parentNode.removeChild(arrClass[arrClass.length-1]);
		console.log("removed:");
		console.log(objRemoved);
	}
}


function showHideHorizontalMenu (objTrigger, objTarget){
	
	if (objTarget.style.display == "block") {
		objTarget.style.display = "none";
	}
	else {
		objTarget.style.display = "block";
		
		var objRectTrigger = objTrigger.getBoundingClientRect();
		var fLeftTrigger = objRectTrigger.left;
		var fTopTrigger = objRectTrigger.top;
		var fHeightTrigger = objRectTrigger.height;
		
		var fTopTarget = fTopTrigger + fHeightTrigger;
		
		console.log(fLeftTrigger);
		console.log(fTopTrigger);
		
		objTarget.style.left = fLeftTrigger.toString() + "px";
		objTarget.style.top = fTopTarget.toString() + "px";
		
		console.log(objTarget.style.left);
		console.log(objTarget.style.top);
	}
}
