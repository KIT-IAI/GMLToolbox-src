<?xml version="1.0" encoding="UTF-8"?>
<schema xmlns="http://purl.oclc.org/dsdl/schematron" queryBinding="xslt2">
    <ns uri="http://www.opengis.net/gml" prefix="gml"/>
    <ns uri="http://www.opengis.net/citygml/building/2.0" prefix="bldg"/>
    
    <pattern  id="CO-bldg-BU-001">
        <rule context="bldg:BuildingPart">
            <assert test="count(../../self::bldg:Building) = 1"><name/> with id <value-of select="@gml:id"/> has no Building as parent</assert>
        </rule>
    </pattern>
    
    <pattern id="CO-bldg-BU-002-1">
        <rule context="bldg:Building | bldg:BuildingPart">
            <assert test="if (child::bldg:lod0FootPrint/descendant::gml:posList) then
                count(tokenize(child::bldg:lod0FootPrint/descendant::gml:posList/normalize-space(.),'\s+')) mod 3 = 0
                else true()">
                <name/> with id <value-of select="@gml:id"/> contains an lod0FootPrint with non-3D geometry (number of coordinates in gml:posList element is not a multiple of 3).
            </assert>
            <assert test="if (child::bldg:lod0FootPrint/descendant::gml:pos) then
                count(tokenize(child::bldg:lod0FootPrint/descendant::gml:pos/normalize-space(.),'\s+')) = 3
                else true()">
                <name/> with id <value-of select="@gml:id"/> contains an lod0FootPrint with non-3D geometry (number of coordinates in gml:pos element is not 3).
            </assert>
        </rule>
    </pattern>
    
    <pattern id="CO-bldg-BU-002-2">
        <rule context="bldg:Building | bldg:BuildingPart">
            <assert test="if (child::bldg:lod0RoofEdge/descendant::gml:posList) then
                count(tokenize(child::bldg:lod0RoofEdge/descendant::gml:posList/normalize-space(.),'\s+')) mod 3 = 0
                else true()">
                <name/> with id <value-of select="@gml:id"/> contains an lod0RoofEdge with non-3D geometry (number of coordinates in gml:posList element is not a multiple of 3).
            </assert>
            <assert test="if (child::bldg:lod0RoofEdge/descendant::gml:pos) then
                count(tokenize(child::bldg:lod0RoofEdge/descendant::gml:pos/normalize-space(.),'\s+')) = 3
                else true()">
                <name/> with id <value-of select="@gml:id"/> contains an lod0RoofEdge with non-3D geometry (number of coordinates in gml:pos element is not 3).
            </assert>
        </rule>
    </pattern>
    
    <pattern id="CO-bldg-BU-003">
        <rule context="bldg:BuildingPart | bldg:Building">
            <report test="child::bldg:lod1Solid and child::bldg:lod1MultiSurface">
                <name/> with id <value-of select="@gml:id"/> consists of both Solid and MultiSurface LoD1 geometry.
            </report>
        </rule>
    </pattern>
    
    <pattern id="CO-bldg-BU-004">
        <rule context="bldg:BuildingPart | bldg:Building">
            <report test="child::bldg:boundedBy/child::bldg:CeilingSurface">
                <name/> with id <value-of select="@gml:id"/> refers to CeilingSurface elements
            </report>
            <report test="child::bldg:boundedBy/child::bldg:InteriorWallSurface">
                <name/> with id <value-of select="@gml:id"/> refers to InteriorWallSurface elements
            </report>
            <report test="child::bldg:boundedBy/child::bldg:FloorSurface">
                <name/> with id <value-of select="@gml:id"/> refers to  FloorSurface elements
            </report>
        </rule>
    </pattern>
    
    <pattern id="CO-bldg-BU-005">
        <rule context="bldg:BuildingPart | bldg:Building">
            <assert test="if (child::bldg:lod2MultiCurve )
                then 
                child::bldg:lod2Solid or 
                child::bldg:lod2MultiSurface or
                child::bldg:boundedBy/child::bldg:lod2MultiSurface
                else true()">
                <name/> with id <value-of select="@gml:id"/> contains a lod2MultiCurve property without other LoD2 geometry
            </assert>
            <assert test="if ( child::bldg:lod3MultiCurve )
                then                
                child::bldg:lod3Solid or 
                child::bldg:lod3MultiSurface or
                child::bldg:boundedBy/child::bldg:lod3MultiSurface
                else true()">
                <name/> with id <value-of select="@gml:id"/> contains a lod3MultiCurve property without other LoD3 geometry
            </assert>
            <assert test="if ( child::bldg:lod4MultiCurve )
                then                 
                child::bldg:lod4Solid or 
                child::bldg:lod4MultiSurface or
                child::bldg:boundedBy/child::bldg:lod4MultiSurface
                else true()">
                <name/> with id <value-of select="@gml:id"/> contains a lod4MultiCurve property without other LoD4 geometry
            </assert>
            
        </rule>
    </pattern>
    
    <pattern id="CO-bldg-BU-006">
        <rule context="bldg:BuildingPart | bldg:Building">
            <assert test="if (child::bldg:outerBuildingInstallation/child::bldg:lod2Geometry or
                              child::bldg:outerBuildingInstallation/child::bldg:lod2ImplicitRepresentation )
                then 
                ( child::bldg:lod2Solid or 
                  child::bldg:lod2MultiSurface or
                  child::bldg:boundedBy/child::bldg:lod2MultiSurface )
                else true()">
                <name/> with id <value-of select="@gml:id"/> has a LoD2 BuildingInstallation, but no LoD2 geometry.
            </assert>
            <assert test="if (child::bldg:outerBuildingInstallation/child::bldg:lod3Geometry or
                              child::bldg:outerBuildingInstallation/child::bldg:lod3ImplicitRepresentation )
                then 
                ( child::bldg:lod3Solid or 
                  child::bldg:lod3MultiSurface or
                  child::bldg:boundedBy/child::bldg:lod3MultiSurface )
                else true()">
                <name/> with id:<value-of select="@gml:id"/> has a LoD3 BuildingInstallation, but no LoD3 geometry.
            </assert>
            <assert test="if (child::bldg:outerBuildingInstallation/child::bldg:lod4Geometry or
                              child::bldg:outerBuildingInstallation/child::bldg:lod4ImplicitRepresentation )
                then 
                ( child::bldg:lod4Solid or 
                 child::bldg:lod4MultiSurface or
                 child::bldg:boundedBy/child::bldg:lod4MultiSurface )
                else true()">
                <name/> with id:<value-of select="@gml:id"/> has a LoD4 BuildingInstallation, but no LoD4 geometry.
            </assert>  
        </rule>
    </pattern>
    
    <pattern id="CO-bldg-BU-007">
        <rule context="bldg:BuildingInstallation">
            <report test="child::bldg:boundedBy/child::bldg:CeilingSurface">
                <name/> with id <value-of select="@gml:id"/> refers to CeilingSurface elements 
            </report>
            <report test="child::bldg:boundedBy/child::bldg:InteriorWallSurface">
                <name/> with id <value-of select="@gml:id"/> refers toInteriorWallSurface elements
            </report>
            <report test="child::bldg:boundedBy/child::bldg:FloorSurface">
                <name/> with id <value-of select="@gml:id"/> refers to FloorSurface elements 
            </report>
        </rule>
    </pattern>
    
    <pattern id="CO-bldg-BU-008">
        <rule context="bldg:RoofSurface | 
                       bldg:WallSurface | 
                       bldg:GroundSurface | 
                       bldg:ClosureSurface | 
                       bldg:CeilingSurface |
                       bldg:InteriorWallSurface | 
                       bldg:FloorSurface | 
                       bldg:OuterCeilingSurface | 
                       bldg:OuterFloorSurface">
            <assert test="if (child::bldg:Door/child::bldg:lod3MultiSurface |
                              child::bldg:Window/child::bldg:lod3MultiSurface |
                              child::bldg:Window/child::bldg:lod3ImplicitRepresentation |
                              child::bldg:Door/child::bldg:lod3ImplicitRepresentation) then
                              child::bldg:lod3MultiSurface else 
                        true()">
                <name/> with id <value-of select="@gml:id"/> has an LoD3 opening, but no LoD3 geometry.
            </assert>
            
            <assert test="if (child::bldg:Door/child::bldg:lod4MultiSurface |
                child::bldg:Window/child::bldg:lod4MultiSurface |
                child::bldg:Window/child::bldg:lod4ImplicitRepresentation |
                child::bldg:Door/child::bldg:lod4ImplicitRepresentation) then
                child::bldg:lod4MultiSurface else 
                true()">
                <name/> with id <value-of select="@gml:id"/> has an LoD4 opening, but no LoD4 geometry.
            </assert>
        </rule>
    </pattern>
    
    <pattern id="CO-bldg-BU-009">
        <rule context="bldg:BuildingPart | bldg:Building">
            <assert test="if (child::bldg:interiorRoom)
                then  
                ( child::bldg:lod4Solid or
                  child::bldg:lod4MultiSurface or
                  child::bldg:boundedBy/child::bldg:lod4MultiSurface )
                else true()">
                <name/> with id <value-of select="@gml:id"/> refers to a Room, but has no LoD 4 geometry.
            </assert>
        </rule>
        
        <rule context="bldg:Room">
            <report test="child::bldg:boundedBy/child::bldg:RoofSurface">
                <name/> with id <value-of select="@gml:id"/> refers to a RoofSurface element 
            </report>
            <report test="child::bldg:boundedBy/child::bldg:WallSurface">
                <name/> with id <value-of select="@gml:id"/> refers to a WallSurface element 
            </report>
            <report test="child::bldg:boundedBy/child::bldg:GroundSurface">
                <name/> with id <value-of select="@gml:id"/> refers to a GroundSurface element 
            </report>
            <report test="child::bldg:boundedBy/child::bldg:OuterCeilingSurface">
                <name/> with id <value-of select="@gml:id"/> refers to an OuterCeilingSurface element 
            </report>
            <report test="child::bldg:boundedBy/child::bldg:OuterFloorSurface">
                <name/> with id <value-of select="@gml:id"/> refers to an OuterFloorSurface element 
            </report>
        </rule>
        
        
        <rule context="bldg:Room">
            <report test="child::bldg:boundedBy/descendant::bldg:lod2MultiSurface">
                <name/> with id  <value-of select="@gml:id"/> refers to a LoD2 BoundarySurface
            </report>
            
            <report test="child::bldg:boundedBy/descendant::bldg:lod3MultiSurface">
                <name/> with id  <value-of select="@gml:id"/> refers to a LoD3 BoundarySurface
            </report>
        </rule>
    </pattern>
    
    <pattern id="CO-bldg-BU-010">
        <rule context="bldg:BuildingPart | bldg:Building">
            <assert test="if (child::bldg:interiorBuildingInstallation)
                then 
                ( child::bldg:lod4Solid or
                  child::bldg:lod4MultiSurface or
                  child::bldg:boundedBy/child::bldg:lod4MultiSurface )
                else true()">
                <name/> with id <value-of select="@gml:id"/> refers to an IntBuildingInstallation, but has no LoD4 geometry
            </assert>
        </rule>
    </pattern>
    
    <pattern id="CO-bldg-BU-011">
        <rule context="bldg:intBuildingInstallation">
            <report test="child::bldg:boundedBy/child::bldg:RoofSurface">
                <name/> with id <value-of select="@gml:id"/> refers to a RoofSurface element 
            </report>
            <report test="child::bldg:boundedBy/child::bldg:WallSurface">
                <name/> with id:<value-of select="@gml:id"/> refers to a WallSurface element
            </report>
            <report test="child::bldg:boundedBy/child::bldg:GroundSurface">
                <name/> with id:<value-of select="@gml:id"/> refers to a GroundSurface element
            </report>
            <report test="child::bldg:boundedBy/child::bldg:OuterCeilingSurface">
                <name/> with id:<value-of select="@gml:id"/> refers to a OuterCeilingSurface element 
            </report>
            <report test="child::bldg:boundedBy/child::bldg:OuterFloorSurface">
                <name/> with id:<value-of select="@gml:id"/> refers to a OuterFloorSurface element 
            </report>
        </rule>
        
        <rule context="bldg:intBuildingInstallation">
            <report test="child::bldg:boundedBy/descendant::bldg:lod2MultiSurface">
                <name/> with id  <value-of select="@gml:id"/> refers to a LoD2 BoundarySurface
            </report>
            
            <report test="child::bldg:boundedBy/descendant::bldg:lod3MultiSurface">
                <name/> with id  <value-of select="@gml:id"/> refers to a LoD3 BoundarySurface
            </report>
        </rule>
        
        
    </pattern>
    
    
    
</schema>