<?xml version="1.0" encoding="UTF-8"?>
<!-- CityGML Version No. 2.0, February 2012 -->
<!-- CityGML - GML 3.1.1 application schema for 3D city models -->
<!-- International encoding standard of the Open Geospatial Consortium, see http://www.opengeospatial.org/standards/citygml -->
<!-- Jointly developed by the Special Interest Group 3D (SIG 3D) of GDI-DE, see http://www.sig3d.org               -->
<!-- For further information see: http://www.citygml.org -->
<schema xmlns="http://www.ascc.net/xml/schematron">
	<title>Schematron validation rules checking referential integrity of CityGML association types</title>
	<ns prefix="gml" uri="http://www.opengis.net/gml"/>
	<ns prefix="xlink" uri="http://www.w3.org/1999/xlink"/>
	<ns prefix="core" uri="http://www.opengis.net/citygml/2.0"/>
	<ns prefix="app" uri="http://www.opengis.net/citygml/appearance/2.0"/>
	<ns prefix="brid" uri="http://www.opengis.net/citygml/bridge/2.0"/>
	<ns prefix="bldg" uri="http://www.opengis.net/citygml/building/2.0"/>
	<ns prefix="frn" uri="http://www.opengis.net/citygml/cityfurniture/2.0"/>
	<ns prefix="grp" uri="http://www.opengis.net/citygml/cityobjectgroup/2.0"/>
	<ns prefix="gen" uri="http://www.opengis.net/citygml/generics/2.0"/>
	<ns prefix="luse" uri="http://www.opengis.net/citygml/landuse/2.0"/>
	<ns prefix="dem" uri="http://www.opengis.net/citygml/relief/2.0"/>
	<ns prefix="tex" uri="http://www.opengis.net/citygml/texturedsurface/2.0"/>
	<ns prefix="tran" uri="http://www.opengis.net/citygml/transportation/2.0"/>
	<ns prefix="tun" uri="http://www.opengis.net/citygml/tunnel/2.0"/>
	<ns prefix="veg" uri="http://www.opengis.net/citygml/vegetation/2.0"/>
	<ns prefix="wtr" uri="http://www.opengis.net/citygml/waterbody/2.0"/>
	<!-- pattern "hrefOrContent" is taken from GML 3.1.1 -->
	<pattern name="Check either href or content not both">
		<rule abstract="true" id="hrefOrContent">
			<report test="@xlink:href and (*|text())">Property element may not carry both a reference to an object and contain an object.</report>
			<assert test="@xlink:href | (*|text())">Property element must either carry a reference to an object or contain an object.</assert>
		</rule>
	</pattern>
	<!-- ========================================================================================= -->
	<!-- =============referential integrity checks on associations of the CityGML Core module==================== -->
	<!-- ========================================================================================= -->
	<pattern name="hrefOrContent check on core:cityObjectMember">
		<rule context="core:cityObjectMember">
			<extends rule="hrefOrContent"/>
		</rule>
	</pattern>
	<pattern name="hrefOrContent check on core:generalizesTo">
		<rule context="core:generalizesTo">
			<extends rule="hrefOrContent"/>
		</rule>
	</pattern>
	<!-- ========================================================================================= -->
	<!-- =============referential integrity checks on associations of the CityGML Appearance module=============== -->
	<!-- ========================================================================================= -->
	<pattern name="hrefOrContent check on app:appearanceMember">
		<rule context="app:appearanceMember">
			<extends rule="hrefOrContent"/>
		</rule>
	</pattern>
	<pattern name="hrefOrContent check on app:appearance">
		<rule context="app:appearance">
			<extends rule="hrefOrContent"/>
		</rule>
	</pattern>
	<pattern name="hrefOrContent check on app:surfaceDataMember">
		<rule context="app:surfaceDataMember">
			<extends rule="hrefOrContent"/>
		</rule>
	</pattern>
	<pattern name="hrefOrContent check on app:target">
		<rule context="app:ParameterizedTexture/app:target">
			<extends rule="hrefOrContent"/>
		</rule>
	</pattern>
	<!-- ========================================================================================= -->
	<!-- =============referential integrity checks on associations of the CityGML Bridge module================== -->
	<!-- ========================================================================================= -->
	<!-- covers boundedBy property of _AbstractBridge, BridgeInstallation, IntBuildingInstallation,
	     BridgeConstructionElement, and BridgeRoom -->
	<pattern name="hrefOrContent check on brid:boundedBy">
		<rule context="brid:boundedBy">
			<extends rule="hrefOrContent"/>
		</rule>
	</pattern>
	<pattern name="hrefOrContent check on brid:outerBridgeConstruction">
		<rule context="brid:outerBridgeConstruction">
			<extends rule="hrefOrContent"/>
		</rule>
	</pattern>
	<pattern name="hrefOrContent check on brid:outerBridgeInstallation">
		<rule context="brid:outerBridgeInstallation">
			<extends rule="hrefOrContent"/>
		</rule>
	</pattern>
	<pattern name="hrefOrContent check on brid:interiorBridgeInstallation">
		<rule context="brid:interiorBridgeInstallation">
			<extends rule="hrefOrContent"/>
		</rule>
	</pattern>
	<pattern name="hrefOrContent check on brid:interiorBridgeRoom">
		<rule context="brid:interiorBridgeRoom">
			<extends rule="hrefOrContent"/>
		</rule>
	</pattern>
	<pattern name="hrefOrContent check on brid:consistsOfBridgePart">
		<rule context="brid:consistsOfBridgePart">
			<extends rule="hrefOrContent"/>
		</rule>
	</pattern>
	<!-- covers address property of _Bridge and Door -->
	<pattern name="hrefOrContent check on brid:address">
		<rule context="brid:address">
			<extends rule="hrefOrContent"/>
		</rule>
	</pattern>
	<pattern name="hrefOrContent check on brid:opening">
		<rule context="brid:opening">
			<extends rule="hrefOrContent"/>
		</rule>
	</pattern>
	<pattern name="hrefOrContent check on brid:interiorFurniture">
		<rule context="brid:interiorFurniture">
			<extends rule="hrefOrContent"/>
		</rule>
	</pattern>
	<pattern name="hrefOrContent check on brid:bridgeRoomInstallation">
		<rule context="brid:bridgeRoomInstallation">
			<extends rule="hrefOrContent"/>
		</rule>
	</pattern>
	<pattern name="hrefOrContent check on brid:lod[1..4]ImplicitRepresentation">
		<rule
			context="brid:lod1ImplicitRepresentation | brid:lod2ImplicitRepresentation | brid:lod3ImplicitRepresentation | brid:lod4ImplicitRepresentation">
			<extends rule="hrefOrContent"/>
		</rule>
	</pattern>
	<!-- ========================================================================================= -->
	<!-- =============referential integrity checks on associations of the CityGML Building module================== -->
	<!-- ========================================================================================= -->
	<!-- covers boundedBy property of _AbstractBuilding, Room, BuildingInstallation, and IntBuildingInstallation -->
	<pattern name="hrefOrContent check on bldg:boundedBy">
		<rule context="bldg:boundedBy">
			<extends rule="hrefOrContent"/>
		</rule>
	</pattern>
	<pattern name="hrefOrContent check on bldg:outerBuildingInstallation">
		<rule context="bldg:outerBuildingInstallation">
			<extends rule="hrefOrContent"/>
		</rule>
	</pattern>
	<pattern name="hrefOrContent check on bldg:interiorBuildingInstallation">
		<rule context="bldg:interiorBuildingInstallation">
			<extends rule="hrefOrContent"/>
		</rule>
	</pattern>
	<pattern name="hrefOrContent check on bldg:interiorRoom">
		<rule context="bldg:interiorRoom">
			<extends rule="hrefOrContent"/>
		</rule>
	</pattern>
	<pattern name="hrefOrContent check on bldg:consistsOfBuildingPart">
		<rule context="bldg:consistsOfBuildingPart">
			<extends rule="hrefOrContent"/>
		</rule>
	</pattern>
	<!-- covers address property of _Building and Door -->
	<pattern name="hrefOrContent check on bldg:address">
		<rule context="bldg:address">
			<extends rule="hrefOrContent"/>
		</rule>
	</pattern>
	<pattern name="hrefOrContent check on bldg:opening">
		<rule context="bldg:opening">
			<extends rule="hrefOrContent"/>
		</rule>
	</pattern>
	<pattern name="hrefOrContent check on bldg:interiorFurniture">
		<rule context="bldg:interiorFurniture">
			<extends rule="hrefOrContent"/>
		</rule>
	</pattern>
	<pattern name="hrefOrContent check on bldg:roomInstallation">
		<rule context="bldg:roomInstallation">
			<extends rule="hrefOrContent"/>
		</rule>
	</pattern>
	<pattern name="hrefOrContent check on bldg:lod[2..4]ImplicitRepresentation">
		<rule context="bldg:lod2ImplicitRepresentation | bldg:lod3ImplicitRepresentation | bldg:lod4ImplicitRepresentation">
			<extends rule="hrefOrContent"/>
		</rule>
	</pattern>
	<!-- ========================================================================================= -->
	<!-- =============referential integrity checks on associations of the CityGML CityFurniture module============== -->
	<!-- ========================================================================================= -->
	<pattern name="hrefOrContent check on frn:lod[1..4]ImplicitRepresentation">
		<rule
			context="frn:lod1ImplicitRepresentation | frn:lod2ImplicitRepresentation | frn:lod3ImplicitRepresentation | frn:lod4ImplicitRepresentation">
			<extends rule="hrefOrContent"/>
		</rule>
	</pattern>
	<!-- ========================================================================================= -->
	<!-- =============referential integrity checks on associations of the CityGML CityObjectGroup module=========== -->
	<!-- ========================================================================================= -->
	<pattern name="hrefOrContent check on grp:groupMember">
		<rule context="grp:groupMember">
			<extends rule="hrefOrContent"/>
		</rule>
	</pattern>
	<pattern name="hrefOrContent check on grp:parent">
		<rule context="grp:parent">
			<extends rule="hrefOrContent"/>
		</rule>
	</pattern>
	<!-- ========================================================================================= -->
	<!-- =============referential integrity checks on associations of the CityGML Generics module================= -->
	<!-- ========================================================================================= -->
	<pattern name="hrefOrContent check on gen:lod[0..4]ImplicitRepresentation">
		<rule
			context="gen:lod0ImplicitRepresentation | gen:lod1ImplicitRepresentation | gen:lod2ImplicitRepresentation |
            gen:lod3ImplicitRepresentation | gen:lod4ImplicitRepresentation">
			<extends rule="hrefOrContent"/>
		</rule>
	</pattern>
	<!-- ========================================================================================= -->
	<!-- =============referential integrity checks on associations of the CityGML Relief module==================== -->
	<!-- ========================================================================================= -->
	<pattern name="hrefOrContent check on dem:reliefComponent">
		<rule context="dem:reliefComponent">
			<extends rule="hrefOrContent"/>
		</rule>
	</pattern>
	<pattern name="hrefOrContent check on dem:tin">
		<rule context="dem:tin">
			<extends rule="hrefOrContent"/>
		</rule>
	</pattern>
	<pattern name="hrefOrContent check on dem:grid">
		<rule context="dem:grid">
			<extends rule="hrefOrContent"/>
		</rule>
	</pattern>
	<!-- ========================================================================================= -->
	<!-- ======referential integrity checks on associations of the CityGML TexturedSurface (deprecated) module======== -->
	<!-- ========================================================================================= -->
	<pattern name="hrefOrContent check on tex:appearance">
		<rule context="tex:appearance">
			<extends rule="hrefOrContent"/>
		</rule>
	</pattern>
	<!-- ========================================================================================= -->
	<!-- =============referential integrity checks on associations of the CityGML Transportation module============= -->
	<!-- ========================================================================================= -->
	<pattern name="hrefOrContent check on tran:trafficArea">
		<rule context="tran:trafficArea">
			<extends rule="hrefOrContent"/>
		</rule>
	</pattern>
	<pattern name="hrefOrContent check on tran:auxiliaryTrafficArea">
		<rule context="tran:auxiliaryTrafficArea">
			<extends rule="hrefOrContent"/>
		</rule>
	</pattern>
	<!-- ========================================================================================= -->
	<!-- =============referential integrity checks on associations of the CityGML Tunnel module================== -->
	<!-- ========================================================================================= -->
	<!-- covers boundedBy property of _AbstractTunnel, TunnelInstallation, IntTunnelInstallation, and HollowSpace -->
	<pattern name="hrefOrContent check on tun:boundedBy">
		<rule context="tun:boundedBy">
			<extends rule="hrefOrContent"/>
		</rule>
	</pattern>
	<pattern name="hrefOrContent check on tun:outerTunnelInstallation">
		<rule context="tun:outerTunnelInstallation">
			<extends rule="hrefOrContent"/>
		</rule>
	</pattern>
	<pattern name="hrefOrContent check on tun:interiorTunnelInstallation">
		<rule context="tun:interiorTunnelInstallation">
			<extends rule="hrefOrContent"/>
		</rule>
	</pattern>
	<pattern name="hrefOrContent check on tun:interiorHollowSpace">
		<rule context="tun:interiorHollowSpace">
			<extends rule="hrefOrContent"/>
		</rule>
	</pattern>
	<pattern name="hrefOrContent check on tun:consistsOfTunnelPart">
		<rule context="tun:consistsOfTunnelPart">
			<extends rule="hrefOrContent"/>
		</rule>
	</pattern>
	<pattern name="hrefOrContent check on tun:opening">
		<rule context="tun:opening">
			<extends rule="hrefOrContent"/>
		</rule>
	</pattern>
	<pattern name="hrefOrContent check on tun:interiorFurniture">
		<rule context="tun:interiorFurniture">
			<extends rule="hrefOrContent"/>
		</rule>
	</pattern>
	<pattern name="hrefOrContent check on tun:hollowSpaceInstallation">
		<rule context="tun:hollowSpaceInstallation">
			<extends rule="hrefOrContent"/>
		</rule>
	</pattern>
	<pattern name="hrefOrContent check on tun:lod[2..4]ImplicitRepresentation">
		<rule context="tun:lod2ImplicitRepresentation | tun:lod3ImplicitRepresentation | tun:lod4ImplicitRepresentation">
			<extends rule="hrefOrContent"/>
		</rule>
	</pattern>
	<!-- ========================================================================================= -->
	<!-- =============referential integrity checks on associations of the CityGML Vegetation module================ -->
	<!-- ========================================================================================= -->
	<pattern name="hrefOrContent check on veg:SolitaryVegetationObject/veg:lod[1..4]ImplicitRepresentation">
		<rule
			context="veg:SolitaryVegetationObject/veg:lod1ImplicitRepresentation | 
            veg:SolitaryVegetationObject/veg:lod2ImplicitRepresentation | 
            veg:SolitaryVegetationObject/veg:lod3ImplicitRepresentation | 
            veg:SolitaryVegetationObject/veg:lod4ImplicitRepresentation">
			<extends rule="hrefOrContent"/>
		</rule>
	</pattern>
	<!-- ========================================================================================= -->
	<!-- =============referential integrity checks on associations of the CityGML WaterBody module================ -->
	<!-- ========================================================================================= -->
	<pattern name="hrefOrContent check on wtr:boundedBy">
		<rule context="wtr:boundedBy">
			<extends rule="hrefOrContent"/>
		</rule>
	</pattern>
</schema>
