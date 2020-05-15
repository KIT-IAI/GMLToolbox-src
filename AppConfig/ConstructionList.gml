<?xml version="1.0" encoding="utf-8" standalone="yes"?>
<!--Erzeugt mit KIT (www.kit.edu) GML-Toolbox, Erstellungsdatum: 11/18/16-->
<CityModel xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
 xsi:schemaLocation="http://www.opengis.net/citygml/2.0 file:/Y:/%5BCityGML%5D/%5BCityGML-Energy%5D/EnergyADE-V_2_1/DataModel/Schema/EnergyADE.xsd"
 xmlns="http://www.opengis.net/citygml/2.0"
 xmlns:grp="http://www.opengis.net/citygml/cityobjectgroup/2.0"
 xmlns:xlink="http://www.w3.org/1999/xlink"
 xmlns:gml="http://www.opengis.net/gml"
 xmlns:energy="http://www.sig3d.org/citygml/2.0/energy/2.0">
  <gml:featureMember>
    <energy:SolidMaterial gml:id="Beton-DIN-12524-2400">
      <gml:description>Beton mit hoher Rohdichte (DIN 12524 - 2400 kg/m³)</gml:description>
     <energy:conductivity uom="W/K*m">2</energy:conductivity>
     <energy:density uom="kg/m3">2400</energy:density>
     <energy:specificHeat uom="kJ/K*kg">1</energy:specificHeat>
    </energy:SolidMaterial>
  </gml:featureMember>
  <gml:featureMember>
    <energy:SolidMaterial gml:id="Bitumendachbahn-DIN-52128">
     <energy:conductivity uom="W/K*m">0.17</energy:conductivity>
     <energy:density uom="kg/m3">1200</energy:density>
     <energy:specificHeat uom="kJ/K*kg">1.5</energy:specificHeat>
    </energy:SolidMaterial>
  </gml:featureMember>
  <gml:featureMember>
    <energy:SolidMaterial gml:id="Bodenplattenmaterial-Gut">
      <gml:description>Bodenplattenmaterial mit guter Wärmedämmung</gml:description>
     <energy:conductivity uom="W/K*m">0.08</energy:conductivity>
     <energy:density uom="kg/m3">2400</energy:density>
     <energy:specificHeat uom="kJ/K*kg">1</energy:specificHeat>
    </energy:SolidMaterial>
  </gml:featureMember>
  <gml:featureMember>
    <energy:SolidMaterial gml:id="Dachmaterial-Gut">
      <gml:description>Dachmaterial mit guter Wärmedämmung</gml:description>
     <energy:conductivity uom="W/K*m">0.15</energy:conductivity>
     <energy:density uom="kg/m3">800</energy:density>
     <energy:specificHeat uom="kJ/K*kg">1</energy:specificHeat>
    </energy:SolidMaterial>
  </gml:featureMember>
  <gml:featureMember>
    <energy:SolidMaterial gml:id="Dachziegelstein-Ton-DIN-12524">
      <gml:description>Dachziegelstein aus Ton nach DIN 12524</gml:description>
     <energy:conductivity uom="W/K*m">1</energy:conductivity>
     <energy:density uom="kg/m3">2000</energy:density>
     <energy:specificHeat uom="kJ/K*kg">0.8</energy:specificHeat>
    </energy:SolidMaterial>
  </gml:featureMember>
  <gml:featureMember>
    <energy:SolidMaterial gml:id="Fenstermaterial-Gut">
      <gml:description>Fenster mit guter Wärmedämmung</gml:description>
     <energy:conductivity uom="W/K*m">0.08</energy:conductivity>
     <energy:density uom="kg/m3">2400</energy:density>
     <energy:specificHeat uom="kJ/K*kg">1</energy:specificHeat>
    </energy:SolidMaterial>
  </gml:featureMember>
  <gml:featureMember>
    <energy:SolidMaterial gml:id="Gipskartonplatte-DIN-18180">
      <gml:description>Gipskartonplatte nach DIN 18180</gml:description>
     <energy:conductivity uom="W/K*m">0.25</energy:conductivity>
     <energy:density uom="kg/m3">900</energy:density>
     <energy:specificHeat uom="kJ/K*kg">1</energy:specificHeat>
    </energy:SolidMaterial>
  </gml:featureMember>
  <gml:featureMember>
    <energy:SolidMaterial gml:id="Hüttenstein-Mauerwerk-1400">
      <gml:description>Hüttenstein-Mauerwerk (1400 kg/m³)</gml:description>
     <energy:conductivity uom="W/K*m">0.99</energy:conductivity>
     <energy:density uom="kg/m3">1800</energy:density>
     <energy:specificHeat uom="kJ/K*kg">1</energy:specificHeat>
    </energy:SolidMaterial>
  </gml:featureMember>
  <gml:featureMember>
    <energy:SolidMaterial gml:id="Konstruktionsholz-DIN-12524-500">
     <energy:conductivity uom="W/K*m">0.13</energy:conductivity>
     <energy:density uom="kg/m3">500</energy:density>
     <energy:specificHeat uom="kJ/K*kg">1.6</energy:specificHeat>
    </energy:SolidMaterial>
  </gml:featureMember>
  <gml:featureMember>
    <energy:SolidMaterial gml:id="Kunstharzputz">
      <gml:description>Kunstharzputz</gml:description>
     <energy:conductivity uom="W/K*m">0.35</energy:conductivity>
     <energy:density uom="kg/m3">1100</energy:density>
     <energy:specificHeat uom="kJ/K*kg">1</energy:specificHeat>
    </energy:SolidMaterial>
  </gml:featureMember>
 
  <gml:featureMember>
    <energy:SolidMaterial gml:id="MineralischeDaemmstoffe-DIN18165">
      <gml:description>MineralischeDaemmstoffe-DIN18165</gml:description>
     <energy:conductivity uom="W/K*m">0.04</energy:conductivity>
     <energy:density uom="kg/m3">260</energy:density>
     <energy:specificHeat uom="kJ/K*kg">1</energy:specificHeat>
    </energy:SolidMaterial>
  </gml:featureMember>
  <gml:featureMember>
    <energy:SolidMaterial gml:id="Polyethylenfolie-DIN-12524-015">
      <gml:description>Polyethylenfolie 0,15mm (DIN 12524)</gml:description>
     <energy:conductivity uom="W/K*m">0.33</energy:conductivity>
     <energy:density uom="kg/m3">960</energy:density>
     <energy:specificHeat uom="kJ/K*kg">1.5</energy:specificHeat>
    </energy:SolidMaterial>
  </gml:featureMember>
  <gml:featureMember>
    <energy:SolidMaterial gml:id="Polystyrol-Extruderschaum-WLG-030">
      <gml:description>Polystyrol-Extruderschaum (WLG 030)</gml:description>
     <energy:conductivity uom="W/K*m">0.03</energy:conductivity>
     <energy:density uom="kg/m3">25</energy:density>
     <energy:specificHeat uom="kJ/K*kg">1.5</energy:specificHeat>
    </energy:SolidMaterial>
  </gml:featureMember>
  <gml:featureMember>
    <energy:SolidMaterial gml:id="Polystyrol-Partikelschaum-WLG-040-20">
      <gml:description>Polystyrol-Partikelschaum (WLG 040 -&gt; 20 kg/m³)</gml:description>
     <energy:conductivity uom="W/K*m">0.04</energy:conductivity>
     <energy:density uom="kg/m3">260</energy:density>
     <energy:specificHeat uom="kJ/K*kg">1</energy:specificHeat>
    </energy:SolidMaterial>
  </gml:featureMember>
  <gml:featureMember>
    <energy:SolidMaterial gml:id="Putzmörtel-1">
      <gml:description>Putzmörtel aus Kalk, Kalzzement, und hydraulischem Kalk</gml:description>
     <energy:conductivity uom="W/K*m">0.7</energy:conductivity>
     <energy:density uom="kg/m3">1400</energy:density>
     <energy:specificHeat uom="kJ/K*kg">1</energy:specificHeat>
    </energy:SolidMaterial>
  </gml:featureMember>
  <gml:featureMember>
    <energy:SolidMaterial gml:id="Wandmaterial-Gut">
      <gml:description>Wandmaterial mit guter Wärmedämmung</gml:description>
     <energy:conductivity uom="W/K*m">0.15</energy:conductivity>
     <energy:density uom="kg/m3">800</energy:density>
     <energy:specificHeat uom="kJ/K*kg">1</energy:specificHeat>
    </energy:SolidMaterial>
  </gml:featureMember>
  <gml:featureMember>
    <energy:SolidMaterial gml:id="Zement-Estrich">
      <gml:description>Zement-Estrich</gml:description>
     <energy:conductivity uom="W/K*m">1.4</energy:conductivity>
     <energy:density uom="kg/m3">2000</energy:density>
     <energy:specificHeat uom="kJ/K*kg">1</energy:specificHeat>
    </energy:SolidMaterial>
  </gml:featureMember>
 
 <gml:featureMember>
  <energy:SolidMaterial gml:id="Bau445-Material-Fassade">
   <gml:description>Fassadenmaterial Bau445 (0.34/1440/1)</gml:description>
   <gml:name>Wall_Material</gml:name>
   <energy:conductivity uom="W/K*m">0.34</energy:conductivity>
   <energy:density uom="kg/m3">1440</energy:density>
   <energy:specificHeat uom="kJ/K*kg">1</energy:specificHeat>
  </energy:SolidMaterial>
 </gml:featureMember>
 
 <gml:featureMember>
  <energy:SolidMaterial gml:id="Bau445-Material-Fassade-SA">
   <gml:description>Fassadenmaterial Bau445 (0.34/1440/1)</gml:description>
   <gml:name>Wall_Material_SA</gml:name>
   <energy:conductivity uom="W/K*m">0.185</energy:conductivity>
   <energy:density uom="kg/m3">800</energy:density>
   <energy:specificHeat uom="kJ/K*kg">1</energy:specificHeat>
  </energy:SolidMaterial>
 </gml:featureMember>
 
 <gml:featureMember>
  <energy:SolidMaterial gml:id="Bau445-Material-Zwischenwand">
   <gml:description>Innenwandmaterial Bau445 (0.6/1200/1)</gml:description>
   <energy:conductivity uom="W/K*m">0.6</energy:conductivity>
   <energy:density uom="kg/m3">1200</energy:density>
   <energy:specificHeat uom="kJ/K*kg">1</energy:specificHeat>
  </energy:SolidMaterial>
 </gml:featureMember>
 
 
 <gml:featureMember>
  <energy:SolidMaterial gml:id="Bau445-Material-Dach">
   <gml:description>Dachmaterial Bau445 (1200/0.22/1)</gml:description>
   <energy:conductivity uom="W/K*m">0.22</energy:conductivity>
   <energy:density uom="kg/m3">1200</energy:density>
   <energy:specificHeat uom="kJ/K*kg">1</energy:specificHeat>
  </energy:SolidMaterial>
 </gml:featureMember>

 <gml:featureMember>
  <energy:SolidMaterial gml:id="Bau445-Material-Dach-SA">
   <gml:description>Dachmaterial Bau445 (1200/0.22/1)</gml:description>
   <gml:name>Dach_Material_SA</gml:name>
   <energy:conductivity uom="W/K*m">0.16</energy:conductivity>
   <energy:density uom="kg/m3">800</energy:density>
   <energy:specificHeat uom="kJ/K*kg">1</energy:specificHeat>
  </energy:SolidMaterial>
 </gml:featureMember>
 
 
 <gml:featureMember>
  <energy:SolidMaterial gml:id="Bau445-Material-Bodenplatte">
   <gml:description>Bodenplattenmaterial Bau445 (24000,18/1)</gml:description>
   <energy:conductivity uom="W/K*m">0.18</energy:conductivity>
   <energy:density uom="kg/m3">2400</energy:density>
   <energy:specificHeat uom="kJ/K*kg">1</energy:specificHeat>
  </energy:SolidMaterial>
 </gml:featureMember>
 
 
 <gml:featureMember>
  <energy:SolidMaterial gml:id="Bau445-Material-Bodenplatte-SA">
   <gml:description>Bodenplattenmaterial Bau445 (24000,18/1)</gml:description>
   <gml:name>Boden_Material_SA</gml:name>
   <energy:conductivity uom="W/K*m">0.085</energy:conductivity>
   <energy:density uom="kg/m3">2400</energy:density>
   <energy:specificHeat uom="kJ/K*kg">1</energy:specificHeat>
  </energy:SolidMaterial>
 </gml:featureMember>
 
 <gml:featureMember>
  <energy:SolidMaterial gml:id="Bau445-Material-Zwischendecke">
   <gml:description>Zwischendeckenmaterial Bau445 (2400/0,18/1)</gml:description>
   <energy:conductivity uom="W/K*m">0.18</energy:conductivity>
   <energy:density uom="kg/m3">2400</energy:density>
   <energy:specificHeat uom="kJ/K*kg">1</energy:specificHeat>
  </energy:SolidMaterial>
 </gml:featureMember>
 
 <gml:featureMember>
  <energy:SolidMaterial gml:id="KIT-FZK-Haus-Putz-Aussen">
   <gml:description>Normalmörtel</gml:description>
   <gml:name>Normalmörtel</gml:name>
   <energy:conductivity uom="W/K*m">1.2</energy:conductivity>
   <energy:density uom="kg/m3">1800</energy:density>
   <energy:specificHeat uom="kJ/K*kg">1.0</energy:specificHeat>
  </energy:SolidMaterial>
 </gml:featureMember>
 
 <gml:featureMember>
  <energy:SolidMaterial gml:id="KIT-FZK-Haus-Porenbeton">
   <gml:description>Porenbeton</gml:description>
   <gml:name>Porenbeton</gml:name>
   <energy:conductivity uom="W/K*m">0.13</energy:conductivity>
   <energy:density uom="kg/m3">400</energy:density>
   <energy:specificHeat uom="kJ/K*kg">1.0</energy:specificHeat>
  </energy:SolidMaterial>
 </gml:featureMember> 
 
 <gml:featureMember>
  <energy:SolidMaterial gml:id="KIT-FZK-Haus-Putz-Innen">
   <gml:description>Putzmörtel (Kalkgips)</gml:description>
   <gml:name>Putzmörtel (Kalkgips)</gml:name>
   <energy:conductivity uom="W/K*m">0.7</energy:conductivity>
   <energy:density uom="kg/m3">1400</energy:density>
   <energy:specificHeat uom="kJ/K*kg">1.0</energy:specificHeat>
  </energy:SolidMaterial>
 </gml:featureMember> 
 
 <gml:featureMember>
  <energy:SolidMaterial gml:id="KIT-FZK-Haus-Kalksandstein">
   <gml:description>Kalksandstein</gml:description>
   <gml:name>Kalksandstein</gml:name>
   <energy:conductivity uom="W/K*m">0.79</energy:conductivity>
   <energy:density uom="kg/m3">1200</energy:density>
   <energy:specificHeat uom="kJ/K*kg">1.0</energy:specificHeat>
  </energy:SolidMaterial>
 </gml:featureMember> 
 
 <gml:featureMember>
  <energy:SolidMaterial gml:id="KIT-FZK-Haus-Beton-Bodenplatte">
   <gml:description>Beton Bodenplatte</gml:description>
   <gml:name>Beton Bodenplatte</gml:name>
   <energy:conductivity uom="W/K*m">2.0</energy:conductivity>
   <energy:density uom="kg/m3">2400</energy:density>
   <energy:specificHeat uom="kJ/K*kg">1.0</energy:specificHeat>
  </energy:SolidMaterial>
 </gml:featureMember> 
 
 <gml:featureMember>
  <energy:SolidMaterial gml:id="KIT-FZK-Haus-Beton-Innendecke">
   <gml:description>Beton Innendecke</gml:description>
   <gml:name>Beton Innendecke</gml:name>
   <energy:conductivity uom="W/K*m">2.3</energy:conductivity>
   <energy:density uom="kg/m3">2200</energy:density>
   <energy:specificHeat uom="kJ/K*kg">1.0</energy:specificHeat>
  </energy:SolidMaterial>
 </gml:featureMember> 
 
 <gml:featureMember>
  <energy:SolidMaterial gml:id="KIT-FZK-Haus-Bitumendachbahn">
   <gml:description>Bitumendachbahn</gml:description>
   <gml:name>Bitumendachbahn</gml:name>
   <energy:conductivity uom="W/K*m">0.17</energy:conductivity>
   <energy:density uom="kg/m3">1200</energy:density>
   <energy:specificHeat uom="kJ/K*kg">1.5</energy:specificHeat>
  </energy:SolidMaterial>
 </gml:featureMember> 
 
 <gml:featureMember>
  <energy:SolidMaterial gml:id="KIT-FZK-Haus-Polystyrol-Extruderschaum">
   <gml:description>Polystyrol-Extruderschaum</gml:description>
   <gml:name>Polystyrol-Extruderschaum</gml:name>
   <energy:conductivity uom="W/K*m">0.035</energy:conductivity>
   <energy:density uom="kg/m3">25</energy:density>
   <energy:specificHeat uom="kJ/K*kg">1.5</energy:specificHeat>
  </energy:SolidMaterial>
 </gml:featureMember> 
 
 <gml:featureMember>
  <energy:SolidMaterial gml:id="KIT-FZK-Haus-Polyethylenfolie">
   <gml:description>Polyethylenfolie</gml:description>
   <gml:name>Polyethylenfolie</gml:name>
   <energy:conductivity uom="W/K*m">0.33</energy:conductivity>
   <energy:density uom="kg/m3">960</energy:density>
   <energy:specificHeat uom="kJ/K*kg">1.5</energy:specificHeat>
  </energy:SolidMaterial>
 </gml:featureMember> 
 
 <gml:featureMember>
  <energy:SolidMaterial gml:id="KIT-FZK-Haus-ZementEstrich">
   <gml:description>Zement-Estrich</gml:description>
   <gml:name>Zement-Estrich</gml:name>
   <energy:conductivity uom="W/K*m">1.4</energy:conductivity>
   <energy:density uom="kg/m3">2000</energy:density>
   <energy:specificHeat uom="kJ/K*kg">1.0</energy:specificHeat>
  </energy:SolidMaterial>
 </gml:featureMember> 
 
 <gml:featureMember>
  <energy:SolidMaterial gml:id="KIT-FZK-Haus-Polystyrol-Partikelschaum">
   <gml:description>Polystyrol-Partikelschaum</gml:description>
   <gml:name>Polystyrol-Partikelschaum</gml:name>
   <energy:conductivity uom="W/K*m">0.04</energy:conductivity>
   <energy:density uom="kg/m3">30</energy:density>
   <energy:specificHeat uom="kJ/K*kg">1.5</energy:specificHeat>
  </energy:SolidMaterial>
 </gml:featureMember> 
 
 <gml:featureMember>
  <energy:SolidMaterial gml:id="KIT-FZK-Haus-PUR-Hartschaum">
   <gml:description>PUR-Hartschaum</gml:description>
   <gml:name>PUR-Hartschaum</gml:name>
   <energy:conductivity uom="W/K*m">0.028</energy:conductivity>
   <energy:density uom="kg/m3">30</energy:density>
   <energy:specificHeat uom="kJ/K*kg">1.0</energy:specificHeat>
  </energy:SolidMaterial>
 </gml:featureMember> 
 
 <gml:featureMember>
  <energy:SolidMaterial gml:id="KIT-FZK-Haus-OSB-Platte">
   <gml:description>OSB-Platte</gml:description>
   <gml:name>OSB-Platte</gml:name>
   <energy:conductivity uom="W/K*m">0.13</energy:conductivity>
   <energy:density uom="kg/m3">650</energy:density>
   <energy:specificHeat uom="kJ/K*kg">1.7</energy:specificHeat>
  </energy:SolidMaterial>
 </gml:featureMember> 
 
 
 <gml:featureMember>
  <energy:SolidMaterial gml:id="KIT-FZK-Haus-Ziegel">
   <gml:description>Dachziegel</gml:description>
   <gml:name>Dachziegel</gml:name>
   <energy:conductivity uom="W/K*m">1.0</energy:conductivity>
   <energy:density uom="kg/m3">2000</energy:density>
   <energy:specificHeat uom="kJ/K*kg">0.8</energy:specificHeat>
  </energy:SolidMaterial>
 </gml:featureMember> 
 
 <gml:featureMember>
  <energy:SolidMaterial gml:id="KIT-FZK-Haus-Holz-Dach">
   <gml:description>Konstruktionsholz Dach</gml:description>
   <gml:name>Konstruktionsholz Dach</gml:name>
   <energy:conductivity uom="W/K*m">0.13</energy:conductivity>
   <energy:density uom="kg/m3">500</energy:density>
   <energy:specificHeat uom="kJ/K*kg">1.6</energy:specificHeat>
  </energy:SolidMaterial>
 </gml:featureMember> 
 
 <gml:featureMember>
  <energy:SolidMaterial gml:id="KIT-FZK-Haus-Holz-Tuer">
   <gml:description>Konstruktionsholz Tür</gml:description>
   <gml:name>Konstruktionsholz Tür</gml:name>
   <energy:conductivity uom="W/K*m">0.18</energy:conductivity>
   <energy:density uom="kg/m3">700</energy:density>
   <energy:specificHeat uom="kJ/K*kg">1.6</energy:specificHeat>
  </energy:SolidMaterial>
 </gml:featureMember> 
  
 <gml:featureMember>
  <energy:Gas gml:id="KIT-FZK-Haus-Luftschicht">
   <gml:description>Luftschicht</gml:description>
   <energy:isVentilated>false</energy:isVentilated>
   <energy:rValue uom="K*m2/W">0.04</energy:rValue>
  </energy:Gas>
 </gml:featureMember>
 
 <gml:featureMember>
  <energy:SolidMaterial gml:id="KIT-FZK-Haus-Gipskarton">
   <gml:description>Gipskartonplatte</gml:description>
   <gml:name>Gipskartonplatte</gml:name>
   <energy:conductivity uom="W/K*m">0.25</energy:conductivity>
   <energy:density uom="kg/m3">900</energy:density>
   <energy:specificHeat uom="kJ/K*kg">1.0</energy:specificHeat>
  </energy:SolidMaterial>
 </gml:featureMember> 
 
 <!--==================================================================== -->
 <!-- Materialien ASHRAE 140-600                                          -->
 <!--==================================================================== -->
 <gml:featureMember>
  <energy:SolidMaterial gml:id="ASHRAE-140-600-Plasterboard">
   <gml:description>ASHRAE-140-600-Plasterboard</gml:description>
   <gml:name>ASHRAE-140-600-Plasterboard</gml:name>
   <energy:conductivity uom="W/K*m">0.160</energy:conductivity>
   <energy:density uom="kg/m3">950</energy:density>
   <energy:specificHeat uom="kJ/K*kg">0.84</energy:specificHeat>
  </energy:SolidMaterial>
 </gml:featureMember>
 
 <gml:featureMember>
  <energy:SolidMaterial gml:id="ASHRAE-140-600-Fiberglass">
   <gml:description>ASHRAE-140-600-Fiberglass</gml:description>
   <gml:name>ASHRAE-140-600-Fiberglass</gml:name>
   <energy:conductivity uom="W/K*m">0.04</energy:conductivity>
   <energy:density uom="kg/m3">12</energy:density>
   <energy:specificHeat uom="kJ/K*kg">0.84</energy:specificHeat>
  </energy:SolidMaterial>
 </gml:featureMember>
 
 <gml:featureMember>
  <energy:SolidMaterial gml:id="ASHRAE-140-600-WoodSiding">
   <gml:description>ASHRAE-140-600-WoodSiding</gml:description>
   <gml:name>ASHRAE-140-600-WoodSiding</gml:name>
   <energy:conductivity uom="W/K*m">0.14</energy:conductivity>
   <energy:density uom="kg/m3">530</energy:density>
   <energy:specificHeat uom="kJ/K*kg">0.9</energy:specificHeat>
  </energy:SolidMaterial>
 </gml:featureMember>
 
 <gml:featureMember>
  <energy:SolidMaterial gml:id="ASHRAE-140-900-ConcreteBlock">
   <gml:description>ASHRAE-140-900-ConcreteBlock</gml:description>
   <gml:name>ASHRAE-140-900-ConcreteBlock</gml:name>
   <energy:conductivity uom="W/K*m">0.51</energy:conductivity>
   <energy:density uom="kg/m3">1400</energy:density>
   <energy:specificHeat uom="kJ/K*kg">1</energy:specificHeat>
  </energy:SolidMaterial>
 </gml:featureMember>
 
 
 <gml:featureMember>
  <energy:SolidMaterial gml:id="ASHRAE-140-900-FoamInsulation">
   <gml:description>ASHRAE-140-900-FoamInsulation</gml:description>
   <gml:name>ASHRAE-140-900-FoamInsulation</gml:name>
   <energy:conductivity uom="W/K*m">0.04</energy:conductivity>
   <energy:density uom="kg/m3">10</energy:density>
   <energy:specificHeat uom="kJ/K*kg">1.4</energy:specificHeat>
  </energy:SolidMaterial>
 </gml:featureMember>
 
 <gml:featureMember>
  <energy:SolidMaterial gml:id="ASHRAE-140-600-RoofDeck">
   <gml:description>ASHRAE-140-600-RoofDeck</gml:description>
   <gml:name>ASHRAE-140-600-RoofDeck</gml:name>
   <energy:conductivity uom="W/K*m">0.14</energy:conductivity>
   <energy:density uom="kg/m3">530</energy:density>
   <energy:specificHeat uom="kJ/K*kg">0.9</energy:specificHeat>
  </energy:SolidMaterial>
 </gml:featureMember> 
  
 <gml:featureMember>
  <energy:SolidMaterial gml:id="ASHRAE-140-600-TimberFlooring">
   <gml:description>ASHRAE-140-600-TimberFlooring</gml:description>
   <gml:name>ASHRAE-140-600-TimberFlooring</gml:name>
   <energy:conductivity uom="W/K*m">0.14</energy:conductivity>
   <energy:density uom="kg/m3">650</energy:density>
   <energy:specificHeat uom="kJ/K*kg">1.2</energy:specificHeat>
  </energy:SolidMaterial>
 </gml:featureMember> 
 
 
 <gml:featureMember>
  <energy:SolidMaterial gml:id="ASHRAE-140-900-ConcreteSlab">
   <gml:description>ASHRAE-140-900-ConcreteSlab</gml:description>
   <gml:name>ASHRAE-140-900-ConcreteSlab</gml:name>
   <energy:conductivity uom="W/K*m">1.13</energy:conductivity>
   <energy:density uom="kg/m3">1400</energy:density>
   <energy:specificHeat uom="kJ/K*kg">1.0</energy:specificHeat>
  </energy:SolidMaterial>
 </gml:featureMember> 
 
 <gml:featureMember>
  <energy:SolidMaterial gml:id="ASHRAE-140-600-Insulation">
   <gml:description>ASHRAE-140-600-Insulation</gml:description>
   <gml:name>ASHRAE-140-600-Insulation</gml:name>
   <energy:conductivity uom="W/K*m">0.04</energy:conductivity>
   <energy:density uom="kg/m3">0</energy:density>
   <energy:specificHeat uom="kJ/K*kg">0</energy:specificHeat>
  </energy:SolidMaterial>
 </gml:featureMember>
 
 <gml:featureMember>
  <energy:SolidMaterial gml:id="ASHRAE-140-600-Glas">
   <gml:description>ASHRAE-140-600-Glas</gml:description>
   <gml:name>ASHRAE-140-600-Glas</gml:name>
   <energy:conductivity uom="W/K*m">1.06</energy:conductivity>
   <energy:density uom="kg/m3">2500</energy:density>
   <energy:specificHeat uom="kJ/K*kg">0.75</energy:specificHeat>
   
   <energy:opticalProperties>
    <energy:OpticalProperties>
     <energy:emissivity>
      <energy:Emissivity>
       <energy:fraction uom="SCALE">0.84</energy:fraction>
       <energy:surface>inside</energy:surface>
      </energy:Emissivity>
     </energy:emissivity>
     
     <energy:emissivity>
      <energy:Emissivity>
       <energy:fraction uom="SCALE">0.84</energy:fraction>
       <energy:surface>outside</energy:surface>
      </energy:Emissivity>
     </energy:emissivity>
     
     <energy:reflectance>
      <energy:Reflectance>
       <energy:fraction uom="SCALE">0.07846</energy:fraction>
       <energy:surface>inside</energy:surface>
       <energy:wavelengthRange>solar</energy:wavelengthRange>
      </energy:Reflectance>
     </energy:reflectance>
     
     <energy:reflectance>
      <energy:Reflectance>
       <energy:fraction uom="SCALE">0.07846</energy:fraction>
       <energy:surface>outside</energy:surface>
       <energy:wavelengthRange>solar</energy:wavelengthRange>
      </energy:Reflectance>
     </energy:reflectance>  
     
     <energy:reflectance>
      <energy:Reflectance>
       <energy:fraction uom="SCALE">0.082</energy:fraction>
       <energy:surface>inside</energy:surface>
       <energy:wavelengthRange>visible</energy:wavelengthRange>
      </energy:Reflectance>
     </energy:reflectance>
     
     <energy:reflectance>
      <energy:Reflectance>
       <energy:fraction uom="SCALE">0.082</energy:fraction>
       <energy:surface>outside</energy:surface>
       <energy:wavelengthRange>visible</energy:wavelengthRange>
      </energy:Reflectance>
     </energy:reflectance>   
     
     <energy:transmittance>
      <energy:Transmittance>
       <energy:fraction uom="SCALE">0.86325</energy:fraction>
       <energy:wavelengthRange>solar</energy:wavelengthRange>
      </energy:Transmittance>
     </energy:transmittance>
     
     <energy:transmittance>
      <energy:Transmittance>
       <energy:fraction uom="SCALE">0.91325</energy:fraction>
       <energy:wavelengthRange>visible</energy:wavelengthRange>
      </energy:Transmittance>
     </energy:transmittance>
     
     <energy:transmittance>
      <energy:Transmittance>
       <energy:fraction uom="SCALE">0.0</energy:fraction>
       <energy:wavelengthRange>infrared</energy:wavelengthRange>
      </energy:Transmittance>
     </energy:transmittance>
    </energy:OpticalProperties>
   </energy:opticalProperties>    
   
  </energy:SolidMaterial>
 </gml:featureMember>
 
 <gml:featureMember>
  <energy:Gas gml:id="ASHRAE-140-600-AirSpace">
   <gml:description>ASHRAE-140-600-AirSpace</gml:description>
   <gml:name>ASHRAE-140-600-AirSpace</gml:name>
   <energy:gasType>air</energy:gasType>
  </energy:Gas>
 </gml:featureMember>
  
 <!--==============================================================================-->
 <!--  IBPSA project 1 DESTEST materials                                           -->
 <!--==============================================================================-->
 <gml:featureMember>
  <energy:SolidMaterial gml:id="DESTEST_HeavyMasonryForExteriorApplications">
  <gml:name>HeavyMasonryForExteriorApplications</gml:name>
  <energy:conductivity uom="W/K*m">1.1</energy:conductivity>
  <energy:density uom="kg/m3">1850</energy:density>
  <energy:specificHeat uom="kJ/K*kg">0.84</energy:specificHeat>
  <energy:opticalProperties>
   <energy:OpticalProperties>
    <energy:absorptance>
     <energy:Absorptance>     
      <energy:fraction uom="SCALE">0.55</energy:fraction>
      <energy:wavelengthRange>solar</energy:wavelengthRange>
     </energy:Absorptance>
    </energy:absorptance>
    
    <energy:absorptance>
     <energy:Absorptance>     
      <energy:fraction uom="SCALE">0.9</energy:fraction>
      <energy:wavelengthRange>infrared</energy:wavelengthRange>
     </energy:Absorptance>
    </energy:absorptance>
   </energy:OpticalProperties>
  </energy:opticalProperties>
 </energy:SolidMaterial> 
 </gml:featureMember>
 
 <gml:featureMember>
  <energy:Gas gml:id="DESTEST_LargeCavityHorizontalHeatTransfer">
   <gml:name>LargeCavityHorizontalHeatTransfer</gml:name>
   <energy:rValue uom="K*m2/W">0.18</energy:rValue>   
  </energy:Gas> 
 </gml:featureMember>
 
 <gml:featureMember>
  <energy:Gas gml:id="DESTEST_LargeCavityVerticalHeatTransfer">
   <gml:name>LargeCavityVerticalHeatTransfer</gml:name>
   <energy:rValue uom="K*m2/W">0.16</energy:rValue>  
  </energy:Gas> 
 </gml:featureMember>
 
 <gml:featureMember>
  <energy:SolidMaterial gml:id="DESTEST_ExpandedPolystrenemOrEPS">
   <gml:name>ExpandedPolystrenemOrEPS</gml:name>
   <energy:conductivity uom="W/K*m">0.036</energy:conductivity>
   <energy:density uom="kg/m3">26</energy:density>
   <energy:specificHeat uom="kJ/K*kg">1.47</energy:specificHeat>
   <energy:opticalProperties>
    <energy:OpticalProperties>
     <energy:absorptance>
      <energy:Absorptance>     
       <energy:fraction uom="SCALE">0.8</energy:fraction>
       <energy:wavelengthRange>solar</energy:wavelengthRange>
      </energy:Absorptance>
     </energy:absorptance>
     
     <energy:absorptance>
      <energy:Absorptance>     
       <energy:fraction uom="SCALE">0.9</energy:fraction>
       <energy:wavelengthRange>infrared</energy:wavelengthRange>
      </energy:Absorptance>
     </energy:absorptance>
    </energy:OpticalProperties>
   </energy:opticalProperties>
  </energy:SolidMaterial> 
 </gml:featureMember>
 
 <gml:featureMember>
  <energy:SolidMaterial gml:id="DESTEST_MediumMasonryForExteriorApplications">
   <gml:name>MediumMasonryForExteriorApplications</gml:name>
   <energy:conductivity uom="W/K*m">0.75</energy:conductivity>
   <energy:density uom="kg/m3">1400</energy:density>
   <energy:specificHeat uom="kJ/K*kg">0.84</energy:specificHeat>
   <energy:opticalProperties>
    <energy:OpticalProperties>
     <energy:absorptance>
      <energy:Absorptance>     
       <energy:fraction uom="SCALE">0.55</energy:fraction>
       <energy:wavelengthRange>solar</energy:wavelengthRange>
      </energy:Absorptance>
     </energy:absorptance>
     
     <energy:absorptance>
      <energy:Absorptance>     
       <energy:fraction uom="SCALE">0.9</energy:fraction>
       <energy:wavelengthRange>infrared</energy:wavelengthRange>
      </energy:Absorptance>
     </energy:absorptance>
    </energy:OpticalProperties>
   </energy:opticalProperties>
  </energy:SolidMaterial> 
 </gml:featureMember>
 
 <gml:featureMember>
  <energy:SolidMaterial gml:id="DESTEST_GypsumPlasterForFinishing">
   <gml:name>GypsumPlasterForFinishing</gml:name>
   <energy:conductivity uom="W/K*m">0.6</energy:conductivity>
   <energy:density uom="kg/m3">975</energy:density>
   <energy:specificHeat uom="kJ/K*kg">0.84</energy:specificHeat>
   <energy:opticalProperties>
    <energy:OpticalProperties>
     <energy:absorptance>
      <energy:Absorptance>     
       <energy:fraction uom="SCALE">0.65</energy:fraction>
       <energy:wavelengthRange>solar</energy:wavelengthRange>
      </energy:Absorptance>
     </energy:absorptance>
     
     <energy:absorptance>
      <energy:Absorptance>     
       <energy:fraction uom="SCALE">0.9</energy:fraction>
       <energy:wavelengthRange>infrared</energy:wavelengthRange>
      </energy:Absorptance>
     </energy:absorptance>
    </energy:OpticalProperties>
   </energy:opticalProperties>
  </energy:SolidMaterial> 
 </gml:featureMember>
 
 <gml:featureMember>
  <energy:SolidMaterial gml:id="DESTEST_DenseCastConcreteAlsoForFinishing">
   <gml:name>DenseCastConcreteAlsoForFinishing</gml:name>
   <energy:conductivity uom="W/K*m">1.4</energy:conductivity>
   <energy:density uom="kg/m3">2100</energy:density>
   <energy:specificHeat uom="kJ/K*kg">0.84</energy:specificHeat>
   <energy:opticalProperties>
    <energy:OpticalProperties>
     <energy:absorptance>
      <energy:Absorptance>     
       <energy:fraction uom="SCALE">0.55</energy:fraction>
       <energy:wavelengthRange>solar</energy:wavelengthRange>
      </energy:Absorptance>
     </energy:absorptance>
     
     <energy:absorptance>
      <energy:Absorptance>     
       <energy:fraction uom="SCALE">0.9</energy:fraction>
       <energy:wavelengthRange>infrared</energy:wavelengthRange>
      </energy:Absorptance>
     </energy:absorptance>
    </energy:OpticalProperties>
   </energy:opticalProperties>
  </energy:SolidMaterial> 
 </gml:featureMember>
 
 <gml:featureMember>
  <energy:SolidMaterial gml:id="DESTEST_ScreedOrLightCastConcrete">
   <gml:name>ScreedOrLightCastConcrete</gml:name>
   <energy:conductivity uom="W/K*m">0.6</energy:conductivity>
   <energy:density uom="kg/m3">1100</energy:density>
   <energy:specificHeat uom="kJ/K*kg">0.84</energy:specificHeat>
   <energy:opticalProperties>
    <energy:OpticalProperties>
     <energy:absorptance>
      <energy:Absorptance>     
       <energy:fraction uom="SCALE">0.55</energy:fraction>
       <energy:wavelengthRange>solar</energy:wavelengthRange>
      </energy:Absorptance>
     </energy:absorptance>
     
     <energy:absorptance>
      <energy:Absorptance>     
       <energy:fraction uom="SCALE">0.9</energy:fraction>
       <energy:wavelengthRange>infrared</energy:wavelengthRange>
      </energy:Absorptance>
     </energy:absorptance>
    </energy:OpticalProperties>
   </energy:opticalProperties>
  </energy:SolidMaterial> 
 </gml:featureMember>
 
 <gml:featureMember>
  <energy:SolidMaterial gml:id="DESTEST_CeramicTileForFinishing">
   <gml:name>CeramicTileForFinishing</gml:name>
   <energy:conductivity uom="W/K*m">1.4</energy:conductivity>
   <energy:density uom="kg/m3">2100</energy:density>
   <energy:specificHeat uom="kJ/K*kg">0.84</energy:specificHeat>
   <energy:opticalProperties>
    <energy:OpticalProperties>
     <energy:absorptance>
      <energy:Absorptance>     
       <energy:fraction uom="SCALE">0.55</energy:fraction>
       <energy:wavelengthRange>solar</energy:wavelengthRange>
      </energy:Absorptance>
     </energy:absorptance>
     
     <energy:absorptance>
      <energy:Absorptance>     
       <energy:fraction uom="SCALE">0.9</energy:fraction>
       <energy:wavelengthRange>infrared</energy:wavelengthRange>
      </energy:Absorptance>
     </energy:absorptance>
    </energy:OpticalProperties>
   </energy:opticalProperties>
  </energy:SolidMaterial> 
 </gml:featureMember>
 
 <gml:featureMember>
  <energy:SolidMaterial gml:id="DESTEST_MediumMasonryForInteriorApplications_A">
   <gml:name>MediumMasonryForInteriorApplications_A</gml:name>
   <energy:conductivity uom="W/K*m">0.54</energy:conductivity>
   <energy:density uom="kg/m3">1400</energy:density>
   <energy:specificHeat uom="kJ/K*kg">0.84</energy:specificHeat>
   <energy:opticalProperties>
    <energy:OpticalProperties>
     <energy:absorptance>
      <energy:Absorptance>     
       <energy:fraction uom="SCALE">0.55</energy:fraction>
       <energy:wavelengthRange>solar</energy:wavelengthRange>
      </energy:Absorptance>
     </energy:absorptance>
     
     <energy:absorptance>
      <energy:Absorptance>     
       <energy:fraction uom="SCALE">0.9</energy:fraction>
       <energy:wavelengthRange>infrared</energy:wavelengthRange>
      </energy:Absorptance>
     </energy:absorptance>
    </energy:OpticalProperties>
   </energy:opticalProperties>
  </energy:SolidMaterial> 
 </gml:featureMember>
 
 <gml:featureMember>
  <energy:SolidMaterial gml:id="DESTEST_TimberForFinishing">
   <gml:name>TimberForFinishing</gml:name>
   <energy:conductivity uom="W/K*m">0.11</energy:conductivity>
   <energy:density uom="kg/m3">550</energy:density>
   <energy:specificHeat uom="kJ/K*kg">1.88</energy:specificHeat>
   <energy:opticalProperties>
    <energy:OpticalProperties>
     <energy:absorptance>
      <energy:Absorptance>     
       <energy:fraction uom="SCALE">0.44</energy:fraction>
       <energy:wavelengthRange>solar</energy:wavelengthRange>
      </energy:Absorptance>
     </energy:absorptance>
     
     <energy:absorptance>
      <energy:Absorptance>     
       <energy:fraction uom="SCALE">0.9</energy:fraction>
       <energy:wavelengthRange>infrared</energy:wavelengthRange>
      </energy:Absorptance>
     </energy:absorptance>
    </energy:OpticalProperties>
   </energy:opticalProperties>
  </energy:SolidMaterial> 
 </gml:featureMember>
 
 
 <gml:featureMember>
  <energy:SolidMaterial gml:id="DESTEST_Glasswool">
   <gml:name>Glasswool</gml:name>
   <energy:conductivity uom="W/K*m">0.04</energy:conductivity>
   <energy:density uom="kg/m3">80</energy:density>
   <energy:specificHeat uom="kJ/K*kg">0.84</energy:specificHeat>
   <energy:opticalProperties>
    <energy:OpticalProperties>
     <energy:absorptance>
      <energy:Absorptance>     
       <energy:fraction uom="SCALE">0.85</energy:fraction>
       <energy:wavelengthRange>solar</energy:wavelengthRange>
      </energy:Absorptance>
     </energy:absorptance>
     
     <energy:absorptance>
      <energy:Absorptance>     
       <energy:fraction uom="SCALE">0.9</energy:fraction>
       <energy:wavelengthRange>infrared</energy:wavelengthRange>
      </energy:Absorptance>
     </energy:absorptance>
    </energy:OpticalProperties>
   </energy:opticalProperties>
  </energy:SolidMaterial> 
 </gml:featureMember>
 
 
 
 
 
 <!--==============================================================================-->
 <!--                           Constructions                                      -->
 <!--==============================================================================-->
 
 <!-- ================================================================================== -->
 <!-- Constructions ASHRAE 140-600                                                       -->
 <!-- ================================================================================== -->
 
 <gml:featureMember>
  <energy:Construction gml:id="ASHRAE-140-600-WallConstruction">
   <gml:description>ASHRAE-140-600-WallConstruction</gml:description>
   <gml:name>ASHRAE-140-600-WallConstruction</gml:name>
   <energy:uValue uom="W/K*m2">0.514</energy:uValue>
   <energy:outsideConvectionCoefficient uom="W/K*m2">29.3</energy:outsideConvectionCoefficient>
   <energy:insideConvectionCoefficient uom="W/K*m2">8.29</energy:insideConvectionCoefficient>
   
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.009</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:material xlink:href="#ASHRAE-140-600-WoodSiding">
       </energy:material>
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer>
   
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.066</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:material xlink:href="#ASHRAE-140-600-Fiberglass">
       </energy:material>
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer>   
   
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.012</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:material xlink:href="#ASHRAE-140-600-Plasterboard">
       </energy:material>
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer>      
  </energy:Construction>
 </gml:featureMember>
 
 <gml:featureMember>
  <energy:Construction gml:id="ASHRAE-140-600-RoofConstruction">
   <gml:description>ASHRAE-140-600-RoofConstruction</gml:description>
   <gml:name>ASHRAE-140-600-RoofConstruction</gml:name>
   <energy:uValue uom="W/K*m2">0.318</energy:uValue>
   <energy:outsideConvectionCoefficient uom="W/K*m2">29.3</energy:outsideConvectionCoefficient>
   <energy:insideConvectionCoefficient uom="W/K*m2">8.29</energy:insideConvectionCoefficient>   
   
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.019</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:material xlink:href="#ASHRAE-140-600-RoofDeck"/>
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer>
   
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.1118</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:material xlink:href="#ASHRAE-140-600-Fiberglass"/>
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer>
   
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.01</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:material xlink:href="#ASHRAE-140-600-Plasterboard"/>
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer>
  </energy:Construction>
 </gml:featureMember>
 
 <gml:featureMember>
  <energy:Construction gml:id="ASHRAE-140-600-FloorConstruction">
   <gml:description>ASHRAE-140-600-FloorConstruction</gml:description>
   <gml:name>ASHRAE-140-600-FloorConstruction</gml:name>
   <energy:uValue uom="W/K*m2">0.039</energy:uValue>
   <energy:insideConvectionCoefficient uom="W/K*m2">8.29</energy:insideConvectionCoefficient>
   
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">1.003</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:material xlink:href="#ASHRAE-140-600-Insulation"/>
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>        
   </energy:layer>
   
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.025</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:material xlink:href="#ASHRAE-140-600-TimberFlooring"/>
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer>
   
  </energy:Construction>
 </gml:featureMember>
 
 <gml:featureMember>
  <energy:Construction gml:id="ASHRAE-140-600-WindowConstruction">
   <energy:uValue uom="W/K*m2">3.0</energy:uValue>
   
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.003175</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:material xlink:href="#ASHRAE-140-600-Glas"/>
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer>
   
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.013</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:material xlink:href="#ASHRAE-140-600-AirSpace"/>
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer>
   
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.003175</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:material xlink:href="#ASHRAE-140-600-Glas"/>
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer>  
   
  </energy:Construction>
 </gml:featureMember>
 
 <gml:featureMember>
  <energy:Construction gml:id="ASHRAE-140-600-WindowConstructionSimple">
   <energy:uValue uom="W/K*m2">3.0</energy:uValue>
   <energy:opticalProperties>    
     <energy:OpticalProperties>
      <energy:transmittance>
       <energy:Transmittance>
        <energy:fraction uom="scale">0.789</energy:fraction>
        <energy:wavelengthRange>total</energy:wavelengthRange>
       </energy:Transmittance>
      </energy:transmittance>
    
     </energy:OpticalProperties>    
   </energy:opticalProperties>     
   <energy:glazingRatio uom="scale">1.0</energy:glazingRatio>
  </energy:Construction>
 </gml:featureMember>
 
 <!-- ================================================================================== -->
 <!-- Constructions ASHRAE 140-900                                                       -->
 <!-- ================================================================================== -->
 
 <gml:featureMember>
  <energy:Construction gml:id="ASHRAE-140-900-WallConstruction">
   <gml:description>ASHRAE-140-900-WallConstruction</gml:description>
   <gml:name>ASHRAE-140-900-WallConstruction</gml:name>
   <energy:uValue uom="W/K*m2">0.512</energy:uValue>
   <energy:outsideConvectionCoefficient uom="W/K*m2">29.3</energy:outsideConvectionCoefficient>
   <energy:insideConvectionCoefficient uom="W/K*m2">8.29</energy:insideConvectionCoefficient>
   
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.009</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:material xlink:href="#ASHRAE-140-600-WoodSiding">
       </energy:material>
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer>
   
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.0615</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:material xlink:href="#ASHRAE-140-900-FoamInsulation">
       </energy:material>
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer>   
   
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.1</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:material xlink:href="#ASHRAE-140-900-ConcreteBlock">
       </energy:material>
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer>      
  </energy:Construction>
 </gml:featureMember>
 
 <gml:featureMember>
  <energy:Construction gml:id="ASHRAE-140-900-FloorConstruction">
   <gml:description>ASHRAE-140-600-FloorConstruction</gml:description>
   <gml:name>ASHRAE-140-600-FloorConstruction</gml:name>
   <energy:uValue uom="W/K*m2">0.039</energy:uValue>
   <energy:insideConvectionCoefficient uom="W/K*m2">8.29</energy:insideConvectionCoefficient>
   
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">1.007</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:material xlink:href="#ASHRAE-140-600-Insulation"/>
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>        
   </energy:layer>
   
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.08</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:material xlink:href="#ASHRAE-140-900-ConcreteSlab"/>
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer>
   
  </energy:Construction>
 </gml:featureMember>
 
 <!-- ================================================================================== -->
 <!--       Constructions FZK-Haus                                                       -->
 <!-- ================================================================================== -->
 
 <gml:featureMember>
  <energy:Construction gml:id="FZK-Haus-Dach">
   <gml:description>FZK-Haus-Dach</gml:description>
   <gml:name>FZK-Haus-Dach</gml:name>
   <energy:uValue uom="W/K*m2">0.25</energy:uValue>
   
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.02</energy:thickness>
     <energy:layerComponent>    
      <energy:LayerComponent>
       <energy:areaFraction uom="scale">1</energy:areaFraction>       
       <energy:material xlink:href="#Dachziegelstein-Ton-DIN-12524" />
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer>
   
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.04</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:areaFraction uom="scale">1</energy:areaFraction>      
       <energy:material xlink:href="#Konstruktionsholz-DIN-12524-500" />
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer>
   
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.04</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:areaFraction uom="scale">1</energy:areaFraction>      
       <energy:material xlink:href="#KIT-FZK-Haus-Luftschicht" />
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer>
   
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.18</energy:thickness>
     <energy:layerComponent>      
      <energy:LayerComponent>
       <energy:areaFraction uom="scale">1</energy:areaFraction>     
       <energy:material xlink:href="#Polystyrol-Partikelschaum-WLG-040-20" />
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer>
   
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.0005</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:areaFraction uom="scale">1</energy:areaFraction>    
       <energy:material xlink:href="#Polyethylenfolie-DIN-12524-015" />
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer>
   
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.025</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:areaFraction uom="scale">1</energy:areaFraction>     
       <energy:material xlink:href="#Gipskartonplatte-DIN-18180" />
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer>   
  </energy:Construction>
 </gml:featureMember>
  
 
 <gml:featureMember>
    <energy:Construction gml:id="_2-Scheiben-Wärmeschutzverglasung-1">
      <gml:description>2-Scheiben Wärmeschutzverglasung 2/1,9/2,2</gml:description>
     <energy:uValue uom="W/K*m2">2.1</energy:uValue>
      <energy:opticalProperties>
        <energy:OpticalProperties>
          <energy:transmittance>
            <energy:Transmittance>
              <energy:fraction uom="scale">0.65</energy:fraction>
              <energy:wavelengthRange>total</energy:wavelengthRange>
            </energy:Transmittance>
          </energy:transmittance>       
        </energy:OpticalProperties>
      </energy:opticalProperties>
     <energy:glazingRatio uom="scale">0.7</energy:glazingRatio>
    </energy:Construction>
  </gml:featureMember>
 
 <gml:featureMember>
  <energy:Construction gml:id="Bau445-Fassade-SingleLayer">
   <gml:description>Construction Außenfassade Bau445 (U-Wert 1,105)</gml:description>
   <energy:uValue uom="W/K*m2">1.105</energy:uValue>
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.25</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:areaFraction uom="scale">1</energy:areaFraction>     
       <energy:material xlink:href="#Bau445-Material-Fassade" />
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer>   
  </energy:Construction>
 </gml:featureMember>
 
 <gml:featureMember>
  <energy:Construction gml:id="Bau445-Fassade-SingleLayer-SA">
   <gml:description>Construction Außenfassade Bau445 (U-Wert 0,558)</gml:description>
   <gml:name>Bau445-Fassade-SingleLayer-SA</gml:name>
   <energy:uValue uom="W/K*m2">0.558</energy:uValue>
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.3</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:areaFraction uom="scale">1</energy:areaFraction>     
       <energy:material xlink:href="#Bau445-Material-Fassade-SA" />
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer>   
  </energy:Construction>
 </gml:featureMember>
 
 <gml:featureMember>
  <energy:Construction gml:id="Bau445-InnerWall-SingleLayer">
   <gml:description>Construction Zwischenwand Bau445 (U-Wert 1.583)</gml:description>
   <energy:uValue uom="W/K*m2">1.583</energy:uValue>
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.2</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:areaFraction uom="scale">1</energy:areaFraction>    
       <energy:material xlink:href="#Bau445-Material-Zwischenwand" />
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer>   
  </energy:Construction>
 </gml:featureMember>
  
 <gml:featureMember>
  <energy:Construction gml:id="Bau445-Roof-SingleLayer">
   <gml:description>Construction Dach Bau445 (U-Wert 0.498)</gml:description>
   <energy:uValue uom="W/K*m2">0.498</energy:uValue>
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.4</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:areaFraction uom="scale">1</energy:areaFraction>      
       <energy:material xlink:href="#Bau445-Material-Dach" />
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer>   
  </energy:Construction>
 </gml:featureMember>

 <gml:featureMember>
  <energy:Construction gml:id="Bau445-Roof-SingleLayer-SA">
   <gml:description>Construction Dach Bau445 (U-Wert 0.484)</gml:description>
   <gml:name>Bau445-Roof-SingleLayer-SA</gml:name>
   <energy:uValue uom="W/K*m2">0.484</energy:uValue>
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.3</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:areaFraction uom="scale">1</energy:areaFraction>      
       <energy:material xlink:href="#Bau445-Material-Dach-SA" />
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer>   
  </energy:Construction>
 </gml:featureMember>

 
 <gml:featureMember>
  <energy:Construction gml:id="Bau445-GroundPlate-SingleLayer">  
   <gml:description>Construction Bodenplatte Bau445 (U-Wert 0.978)</gml:description>
   <energy:uValue uom="W/K*m2">0.978</energy:uValue>
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.15</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:areaFraction uom="scale">1</energy:areaFraction>   
       <energy:material xlink:href="#Bau445-Material-Bodenplatte" />
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer>   
  </energy:Construction>
 </gml:featureMember>
 
 <gml:featureMember>
  <energy:Construction gml:id="Bau445-GroundPlate-SingleLayer-SA">  
   <gml:description>Construction Bodenplatte Bau445 (U-Wert 0.393)</gml:description>
   <gml:name>Bau445-GroundPlate-SingleLayer-SA</gml:name>
   <energy:uValue uom="W/K*m2">0.393</energy:uValue>
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.2</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:areaFraction uom="scale">1</energy:areaFraction>   
       <energy:material xlink:href="#Bau445-Material-Bodenplatte-SA" />
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer>   
  </energy:Construction>
 </gml:featureMember>
 
 <gml:featureMember>
  <energy:Construction gml:id="Bau445-InnerSlab-SingleLayer">
   <gml:description>Construction Zwischendecke Bau445 (U-Wert 0.884)</gml:description>
   <energy:uValue uom="W/K*m2">0.884</energy:uValue>
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.15</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:areaFraction uom="scale">1</energy:areaFraction>       
       <energy:material xlink:href="#Bau445-Material-Zwischendecke" />
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer>   
  </energy:Construction>
 </gml:featureMember>
 
 <gml:featureMember>
  <energy:Construction gml:id="Bau445-Window">
   <gml:description>2-Construction Fenster Bau445</gml:description>
   <energy:uValue uom="W/K*m2">1.4</energy:uValue>
   <energy:opticalProperties>
    <energy:OpticalProperties>
     <energy:transmittance>
      <energy:Transmittance>
       <energy:fraction uom="scale">0.78</energy:fraction>
       <energy:wavelengthRange>total</energy:wavelengthRange>
      </energy:Transmittance>
     </energy:transmittance> 
    </energy:OpticalProperties>
   </energy:opticalProperties>
   <energy:glazingRatio uom="scale">0.7</energy:glazingRatio>
  </energy:Construction>
 </gml:featureMember>
 
 <gml:featureMember>
  <energy:Construction gml:id="Bau445-Window-SA">
   <gml:description>2-Construction Fenster Bau445</gml:description>
   <gml:name>Bau445-Window-SA</gml:name>
   <energy:uValue uom="W/K*m2">1.8</energy:uValue>
   <energy:opticalProperties>
    <energy:OpticalProperties>
     <energy:transmittance>
      <energy:Transmittance>
       <energy:fraction uom="scale">0.6</energy:fraction>
       <energy:wavelengthRange>total</energy:wavelengthRange>
      </energy:Transmittance>
     </energy:transmittance> 
    </energy:OpticalProperties>
   </energy:opticalProperties>
   <energy:glazingRatio uom="scale">0.7</energy:glazingRatio>
  </energy:Construction>
 </gml:featureMember>
 
 <gml:featureMember>
  <energy:Construction gml:id="Bau445-Door">
   <gml:description>2-Construction Tür Bau445</gml:description>
   <energy:uValue uom="W/K*m2">1.4</energy:uValue>
   <energy:opticalProperties>
    <energy:OpticalProperties>
     <energy:transmittance>
      <energy:Transmittance>
       <energy:fraction uom="scale">0.78</energy:fraction>
       <energy:wavelengthRange>total</energy:wavelengthRange>
      </energy:Transmittance>
     </energy:transmittance>   
    </energy:OpticalProperties>
   </energy:opticalProperties>
   <energy:glazingRatio uom="scale">0.9</energy:glazingRatio>
  </energy:Construction>
 </gml:featureMember>
 
 <!-- =================================================================================== -->
 <!--                     Constructions FZK-Haus                                          -->
 <!-- =================================================================================== -->
  <gml:featureMember>
  <energy:Construction gml:id="KIT-FZK-Haus-Aussenwand">
   <gml:description>KIT-FZK-Haus-Aussenwand</gml:description>
   <gml:name>KIT-FZK-Haus-Aussenwand</gml:name>
   <energy:uValue uom="W/K*m2">0.4</energy:uValue>
   
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.02</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:areaFraction uom="scale">1.0</energy:areaFraction>     
       <energy:material xlink:href="#KIT-FZK-Haus-Putz-Aussen"/>
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer> 
   
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.2</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:areaFraction uom="scale">1.0</energy:areaFraction>      
       <energy:material xlink:href="#KIT-FZK-Haus-Porenbeton"/>
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer>   
   
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.015</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:areaFraction uom="scale">1.0</energy:areaFraction>     
       <energy:material xlink:href="#KIT-FZK-Haus-Putz-Innen"/>
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer>
   
  </energy:Construction>
 </gml:featureMember>
 
 <gml:featureMember>
  <energy:Construction gml:id="KIT-FZK-Haus-Innenwand">
   <gml:description>KIT-FZK-Haus-Innenwand</gml:description>
   <gml:name>KIT-FZK-Haus-Innenwand</gml:name>
   <energy:uValue uom="W/K*m2">2.3</energy:uValue>
   
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.01</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:areaFraction uom="scale">1.0</energy:areaFraction>      
       <energy:material xlink:href="#KIT-FZK-Haus-Putz-Innen"/>
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer> 
   
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.115</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:areaFraction uom="scale">1.0</energy:areaFraction>      
       <energy:material xlink:href="#KIT-FZK-Haus-Kalksandstein"/>
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer> 
   
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.01</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:areaFraction uom="scale">1.0</energy:areaFraction>      
       <energy:material xlink:href="#KIT-FZK-Haus-Putz-Innen"/>
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer> 
   
  </energy:Construction>
 </gml:featureMember>
 
 <gml:featureMember>
  <energy:Construction gml:id="KIT-FZK-Haus-Innendecke">
   <gml:description>KIT-FZK-Haus-Innendecke</gml:description>
   <gml:name>KIT-FZK-Haus-Innendecke</gml:name>
   <energy:uValue uom="W/K*m2">0.18</energy:uValue>
   
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.015</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:areaFraction uom="scale">1.0</energy:areaFraction>       
       <energy:material xlink:href="#KIT-FZK-Haus-Putz-Innen"/>
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer> 
   
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.14</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:areaFraction uom="scale">1.0</energy:areaFraction>      
       <energy:material xlink:href="#KIT-FZK-Haus-Beton-Innendecke"/>
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer> 
   
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.0002</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:areaFraction uom="scale">1.0</energy:areaFraction>      
       <energy:material xlink:href="#KIT-FZK-Haus-Polyethylenfolie"/>
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer> 
   
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.14</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:areaFraction uom="scale">1.0</energy:areaFraction>       
       <energy:material xlink:href="#KIT-FZK-Haus-PUR-Hartschaum"/>
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer> 
   
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.018</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:areaFraction uom="scale">1.0</energy:areaFraction>       
       <energy:material xlink:href="#KIT-FZK-Haus-OSB-Platte"/>
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer>   
   
   </energy:Construction>
 </gml:featureMember>
 
 <gml:featureMember>
  <energy:Construction gml:id="KIT-FZK-Haus-Dach">
   <gml:description>KIT-FZK-Haus-Dach</gml:description>
   <gml:name>KIT-FZK-Haus-Dach</gml:name>
   <energy:uValue uom="W/K*m2">0.4</energy:uValue>
   
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.02</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:areaFraction uom="scale">1.0</energy:areaFraction>       
       <energy:material xlink:href="#KIT-FZK-Haus-Ziegel"/>
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer>   
   
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.04</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:areaFraction uom="scale">1.0</energy:areaFraction>      
       <energy:material xlink:href="#KIT-FZK-Haus-Holz-Dach"/>
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer>   
   
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.04</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:areaFraction uom="scale">1.0</energy:areaFraction>       
       <energy:material xlink:href="#KIT-FZK-Haus-Luftschicht"/>
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer>   
   
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.08</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:areaFraction uom="scale">1</energy:areaFraction>      
       <energy:material xlink:href="#KIT-FZK-Haus-Polystyrol-Partikelschaum"/>
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer>   
   
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.0005</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:areaFraction uom="scale">1.0</energy:areaFraction>       
       <energy:material xlink:href="#KIT-FZK-Haus-Polyethylenfolie"/>
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer>   
   
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.06</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:areaFraction uom="scale">1.0</energy:areaFraction>       
       <energy:material xlink:href="#KIT-FZK-Haus-Polystyrol-Partikelschaum"/>
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer>   
   
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.025</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:areaFraction uom="scale">1.0</energy:areaFraction>      
       <energy:material xlink:href="#KIT-FZK-Haus-Gipskarton"/>
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer>     
   
  </energy:Construction>
 </gml:featureMember>
 
 <gml:featureMember>
  <energy:Construction gml:id="KIT-FZK-Haus-Bodenplatte">
   <gml:description>KIT-FZK-Haus-Bodenplatte</gml:description>
   <gml:name>KIT-FZK-Haus-Bodenplatte</gml:name>
   <energy:uValue uom="W/K*m2">0.26</energy:uValue>
   
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.12</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:areaFraction uom="scale">1.0</energy:areaFraction>    
       <energy:material xlink:href="#KIT-FZK-Haus-Polystyrol-Extruderschaum"/>
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer>     
   
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.0005</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:areaFraction uom="scale">1.0</energy:areaFraction>       
       <energy:material xlink:href="#KIT-FZK-Haus-Bitumendachbahn"/>
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer>     
    
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.1</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:areaFraction uom="scale">1.0</energy:areaFraction>      
       <energy:material xlink:href="#KIT-FZK-Haus-Beton-Bodenplatte"/>
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer>     
   
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.01</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:areaFraction uom="scale">1.0</energy:areaFraction>       
       <energy:material xlink:href="#KIT-FZK-Haus-Polystyrol-Partikelschaum"/>
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer>     
   
   
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.0002</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:areaFraction uom="scale">1.0</energy:areaFraction>     
       <energy:material xlink:href="#KIT-FZK-Haus-Polyethylenfolie"/>
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer>      
   
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.02</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:areaFraction uom="scale">1.0</energy:areaFraction>      
       <energy:material xlink:href="#KIT-FZK-Haus-ZementEstrich"/>
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer>     
   
  </energy:Construction>
 </gml:featureMember>
 
 <gml:featureMember>
  <energy:Construction gml:id="KIT-FZK-Haus-Innentuer">
   <gml:description>KIT-FZK-Haus-Innentuer</gml:description>
   <gml:name>KIT-FZK-Haus-Innentuer</gml:name>
   <energy:uValue uom="W/K*m2">2.23</energy:uValue>
   
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.06</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:areaFraction uom="scale">1.0</energy:areaFraction>       
       <energy:material xlink:href="#KIT-FZK-Haus-Holz-Tuer"/>
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer>      
   
  </energy:Construction>
 </gml:featureMember>
 
 <gml:featureMember>
  <energy:Construction gml:id="KIT-FZK-Haus-Haustuer">
   <gml:description>KIT-FZK-Haus-Haustuer</gml:description>
   <gml:name>KIT-FZK-Haus-Haustuer</gml:name>
   <energy:uValue uom="W/K*m2">1.99</energy:uValue>
   
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.06</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:areaFraction uom="scale">1.0</energy:areaFraction>      
       <energy:material xlink:href="#KIT-FZK-Haus-Holz-Tuer"/>
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer>      
   
  </energy:Construction>
 </gml:featureMember>
 
 <gml:featureMember>
  <energy:Construction gml:id="KIT-FZK-Haus-Fenster">
   <gml:description>KIT-FZK-Haus-Fenster</gml:description>
   <gml:name>KIT-FZK-Haus-Fenster</gml:name>
   <energy:uValue uom="W/K*m2">1.2</energy:uValue>
   <energy:opticalProperties>
    <energy:OpticalProperties>
     <energy:transmittance>
      <energy:Transmittance>
       <energy:fraction uom="scale">0.6</energy:fraction>
       <energy:wavelengthRange>total</energy:wavelengthRange>
      </energy:Transmittance>
     </energy:transmittance>   
    </energy:OpticalProperties>
   </energy:opticalProperties>
   <energy:glazingRatio uom="scale">0.7</energy:glazingRatio>
  </energy:Construction>
 </gml:featureMember>
 
 <gml:featureMember>
  <energy:Construction gml:id="KIT-FZK-Haus-Terrassentuer">
   <gml:description>KIT-FZK-Haus-Terrassentuer</gml:description>
   <gml:name>KIT-FZK-Haus-Terrassentuer</gml:name>
   <energy:uValue uom="W/K*m2">1.2</energy:uValue>
   <energy:opticalProperties>
    <energy:OpticalProperties>
     <energy:transmittance>
      <energy:Transmittance>
       <energy:fraction uom="scale">0.6</energy:fraction>
       <energy:wavelengthRange>total</energy:wavelengthRange>
      </energy:Transmittance>
     </energy:transmittance>  
    </energy:OpticalProperties>
   </energy:opticalProperties>
   <energy:glazingRatio uom="scale">0.7</energy:glazingRatio>
  </energy:Construction>
 </gml:featureMember>
 
 <!--============================================================================ -->
 <!--  DESTEST Construtions                                                       -->
 <!--============================================================================ -->
 <gml:featureMember>
  <energy:Construction gml:id="DESTEST_OuterWall">
   <gml:description>DESTEST_OuterWall</gml:description>
   <gml:name>DESTEST_OuterWall</gml:name>
   <energy:outsideConvectionCoefficient uom="W/K*m2">20</energy:outsideConvectionCoefficient>
   <energy:insideConvectionCoefficient uom="W/K*m2">2.7</energy:insideConvectionCoefficient>   
   
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.1</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:material xlink:href="#DESTEST_HeavyMasonryForExteriorApplications"/>
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer>
   
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.1</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:material xlink:href="#DESTEST_LargeCavityHorizontalHeatTransfer"/>
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer>
   
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.01</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:material xlink:href="#DESTEST_ExpandedPolystrenemOrEPS"/>
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer>
   
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.14</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:material xlink:href="#DESTEST_MediumMasonryForExteriorApplications"/>
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer>
   
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.02</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:material xlink:href="#DESTEST_GypsumPlasterForFinishing"/>
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer>
   
  </energy:Construction>
 </gml:featureMember>
 
 <gml:featureMember>
  <energy:Construction gml:id="DESTEST_GroundPlate">
   <gml:description>DESTEST_GroundPlate</gml:description>
   <gml:name>DESTEST_GroundPlate</gml:name>  
   <energy:insideConvectionCoefficient uom="W/K*m2">1.7</energy:insideConvectionCoefficient>   
   
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.15</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:material xlink:href="#DESTEST_DenseCastConcreteAlsoForFinishing"/>
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer>
   
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.03</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:material xlink:href="#DESTEST_ExpandedPolystrenemOrEPS"/>
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer>
   
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.08</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:material xlink:href="#DESTEST_ScreedOrLightCastConcrete"/>
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer>
   
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.02</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:material xlink:href="#DESTEST_CeramicTileForFinishing"/>
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer>
   
  </energy:Construction>
 </gml:featureMember>
 
 <gml:featureMember>
  <energy:Construction gml:id="DESTEST_InteriorWall">
   <gml:description>DESTEST_InteriorWall</gml:description>
   <gml:name>DESTEST_InteriorWall</gml:name> 
   <energy:insideConvectionCoefficient uom="W/K*m2">1.7</energy:insideConvectionCoefficient>   
   
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.02</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:material xlink:href="#DESTEST_GypsumPlasterForFinishing"/>
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer>
   
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.14</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:material xlink:href="#DESTEST_MediumMasonryForInteriorApplications_A"/>
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer>
   
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.02</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:material xlink:href="#DESTEST_GypsumPlasterForFinishing"/>
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer>
   
  </energy:Construction>
 </gml:featureMember>
 
 <gml:featureMember>
  <energy:Construction gml:id="DESTEST_Ceiling">
   <gml:description>DESTEST_Ceiling</gml:description>
   <gml:name>DESTEST_Ceiling</gml:name>   
   <energy:insideConvectionCoefficient uom="W/K*m2">1.7</energy:insideConvectionCoefficient>   
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.02</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:material xlink:href="#DESTEST_TimberForFinishing"></energy:material>
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer>
   
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.08</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:material xlink:href="#DESTEST_ScreedOrLightCastConcrete"></energy:material>
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer>
   
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.2</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:material xlink:href="#DESTEST_DenseCastConcreteAlsoForFinishing"></energy:material>
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer>
     
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.02</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:material xlink:href="#DESTEST_GypsumPlasterForFinishing"></energy:material>
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer>   
   
  </energy:Construction>
 </gml:featureMember>
 
 <gml:featureMember>
  <energy:Construction gml:id="DESTEST_Floor">
   <gml:description>DESTEST_Floor</gml:description>
   <gml:name>DESTEST_Floor</gml:name>  
   <energy:insideConvectionCoefficient uom="W/K*m2">1.7</energy:insideConvectionCoefficient>   
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.02</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:material xlink:href="#DESTEST_GypsumPlasterForFinishing"></energy:material>
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer>   
   
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.2</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:material xlink:href="#DESTEST_DenseCastConcreteAlsoForFinishing"></energy:material>
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer>
   
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.08</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:material xlink:href="#DESTEST_ScreedOrLightCastConcrete"></energy:material>
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer>   
   
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.02</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:material xlink:href="#DESTEST_TimberForFinishing"></energy:material>
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer>   
    
  </energy:Construction>
 </gml:featureMember>
 
 <gml:featureMember>
  <energy:Construction gml:id="DESTEST_Roof">
   <gml:description>DESTEST_Roof</gml:description>
   <gml:name>DESTEST_Roof</gml:name>
   <energy:outsideConvectionCoefficient uom="W/K*m2">20</energy:outsideConvectionCoefficient>
   <energy:insideConvectionCoefficient uom="W/K*m2">1.7</energy:insideConvectionCoefficient>   
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.025</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:material xlink:href="#DESTEST_CeramicTileForFinishing"/>
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer>
   
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.1</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:material xlink:href="#DESTEST_LargeCavityVerticalHeatTransfer"/>
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer>
   
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.04</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:material xlink:href="#DESTEST_Glasswool"/>
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer>
   
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.02</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:material xlink:href="#DESTEST_GypsumPlasterForFinishing"/>
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer>  
   
  </energy:Construction>
 </gml:featureMember>
 
 <gml:featureMember>
  <energy:Construction gml:id="DESTEST_Window">
   <gml:description>DESTEST_Window</gml:description>
   <gml:name>DESTEST_Window</gml:name>
   <energy:uValue uom="W/K*m2">3.15</energy:uValue>
   
   <energy:opticalProperties>
    <energy:OpticalProperties>
     <energy:transmittance>
      <energy:Transmittance>
       <energy:fraction uom="SCALE">0.78</energy:fraction>
       <energy:wavelengthRange>total</energy:wavelengthRange>
      </energy:Transmittance>
     </energy:transmittance>
    </energy:OpticalProperties>
   </energy:opticalProperties>
   <energy:glazingRatio uom="SCALE">0.85</energy:glazingRatio>
   <energy:outsideConvectionCoefficient uom="W/K*m2">20</energy:outsideConvectionCoefficient>
   <energy:insideConvectionCoefficient uom="W/K*m2">1.7</energy:insideConvectionCoefficient>   
   
   <energy:layer>
    <energy:Layer>
     <energy:thickness uom="m">0.005</energy:thickness>
     <energy:layerComponent>
      <energy:LayerComponent>
       <energy:material xlink:href="#ASHRAE-140-600-Glas"/>
      </energy:LayerComponent>
     </energy:layerComponent>
    </energy:Layer>
   </energy:layer>
   
  </energy:Construction>
 </gml:featureMember>
  
 
 <gml:featureMember>
  <grp:CityObjectGroup>
   <gml:name>ASHRAE-140-600</gml:name>
   <grp:groupMember xlink:title="Fassade" xlink:href="#ASHRAE-140-600-WallConstruction"/>
   <grp:groupMember xlink:title="Roof" xlink:href="#ASHRAE-140-600-RoofConstruction"/>
   <grp:groupMember xlink:title="GroundPlate" xlink:href="#ASHRAE-140-600-FloorConstruction"/>
   <grp:groupMember xlink:title="Window" xlink:href="#ASHRAE-140-600-WindowConstruction"/>   
  </grp:CityObjectGroup>
 </gml:featureMember>
 
 <gml:featureMember>
  <grp:CityObjectGroup>
   <gml:name>ASHRAE-140-900</gml:name>
   <grp:groupMember xlink:title="Fassade" xlink:href="#ASHRAE-140-900-WallConstruction"/>
   <grp:groupMember xlink:title="Roof" xlink:href="#ASHRAE-140-600-RoofConstruction"/>
   <grp:groupMember xlink:title="GroundPlate" xlink:href="#ASHRAE-140-900-FloorConstruction"/>
   <grp:groupMember xlink:title="Window" xlink:href="#ASHRAE-140-600-WindowConstruction"/>  
  </grp:CityObjectGroup>
 </gml:featureMember>
 
 <gml:featureMember>
  <grp:CityObjectGroup>
   <gml:name>KIT-FZK-Haus</gml:name>
   <grp:groupMember xlink:title="Fassade" xlink:href="#KIT-FZK-Haus-Aussenwand"/> 
   <grp:groupMember xlink:title="Roof" xlink:href="#KIT-FZK-Haus-Dach"/>
   <grp:groupMember xlink:title="GroundPlate" xlink:href="#KIT-FZK-Haus-Bodenplatte"/>
   <grp:groupMember xlink:title="InnerWall" xlink:href="#KIT-FZK-Haus-Innenwand"/>
   <grp:groupMember xlink:title="InnerSlab" xlink:href="#KIT-FZK-Haus-Innendecke"/>
   <grp:groupMember xlink:title="OuterDoor" xlink:href="#KIT-FZK-Haus-Terrassentuer"/>
   <grp:groupMember xlink:title="InnerDoor" xlink:href="KIT-FZK-Haus-Innentuer"/>
   <grp:groupMember xlink:title="Window" xlink:href="#KIT-FZK-Haus-Fenster"/>
   
   <energy:openingFractionRoof uom="scale">0</energy:openingFractionRoof>
   <energy:openingFractionWall uom="scale">0.3</energy:openingFractionWall>
  </grp:CityObjectGroup>
 </gml:featureMember>
  
 <gml:featureMember>
  <grp:CityObjectGroup>
   <gml:name>Building 445 - Single Layer Constrictions</gml:name>
   <grp:groupMember xlink:title="Fassade" xlink:href="#Bau445-Fassade-SingleLayer"/>
   <grp:groupMember xlink:title="Roof" xlink:href="#Bau445-Roof-SingleLayer"/>
   <grp:groupMember xlink:title="GroundPlate" xlink:href="#Bau445-GroundPlate-SingleLayer"/>
   <grp:groupMember xlink:title="InnerWall" xlink:href="#Bau445-InnerWall-SingleLayer"/>
   <grp:groupMember xlink:title="InnerSlab" xlink:href="#Bau445-InnerSlab-SingleLayer"/>
   <grp:groupMember xlink:title="OuterDoor" xlink:href="#Bau445-Door"/>
   <grp:groupMember xlink:title="Window" xlink:href="#Bau445-Window"/>
   
   <energy:openingFractionWall uom="scale">0.2</energy:openingFractionWall>
   <energy:openingFractionRoof uom="scale">0</energy:openingFractionRoof>
  </grp:CityObjectGroup>
 </gml:featureMember>
 
 <gml:featureMember>
  <grp:CityObjectGroup>
   <gml:name>Building 445 - Single Layer Constrictions - SA</gml:name>
   <grp:groupMember xlink:title="Fassade" xlink:href="#Bau445-Fassade-SingleLayer-SA"/>
   <grp:groupMember xlink:title="Roof" xlink:href="#Bau445-Roof-SingleLayer-SA"/>
   <grp:groupMember xlink:title="GroundPlate" xlink:href="#Bau445-GroundPlate-SingleLayer-SA"/>
   <grp:groupMember xlink:title="Window" xlink:href="#Bau445-Window-SA"/>   
   <energy:openingFractionWall uom="scale">0.2</energy:openingFractionWall>
   <energy:openingFractionRoof uom="scale">0</energy:openingFractionRoof>
  </grp:CityObjectGroup>
 </gml:featureMember>
 
 <gml:featureMember>
  <grp:CityObjectGroup>
   <gml:name>DESTEST</gml:name>
   <grp:groupMember xlink:title="Fassade" xlink:href="#DESTEST_OuterWall"/>
   <grp:groupMember xlink:title="Roof" xlink:href="#DESTEST_Roof"/>
   <grp:groupMember xlink:title="GroundPlate" xlink:href="#DESTEST_GroundPlate"/>
   <grp:groupMember xlink:title="InnerWall" xlink:href="#DESTEST_InteriorWall"/>
   <grp:groupMember xlink:title="InnerSlab" xlink:href="#DESTEST_Floor"/>
   <grp:groupMember xlink:title="Window" xlink:href="#DESTEST_Window"/>   
  </grp:CityObjectGroup>
 </gml:featureMember>
  
</CityModel>