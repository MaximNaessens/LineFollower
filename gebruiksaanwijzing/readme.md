# Gebruiksaanwijzing

### opladen / vervangen batterijen
Ik heb gekozen voor oplaadbare batterijen (18650) aangezien je er anders heel veel nodig hebt. Met twee volledig opgeladen batterijen kan je ongeveer een vol uur rijden. Om de batterijen op te laden haal je deze uit de batterijhouder op de auto. steek je ze in de oplader die hiervoor is voorzien. Als het lichtje groen brandt is de batterij volledig opgeladen. 

### draadloze communicatie HC-05
#### verbinding maken
(Werkt enkel met android of windows)
De robot met uw gsm verbinden doe je als volgt: ga naar instellingen en bij bluetooth zou je uw HC-05 module moeten zien staan. klik op de HC-05 module, normaal gezien moet je kunnen verbinden. Ga naar de serial bluetooth terminal op uw smarthphone en bij devices zou normaal de HC-05 er tussen moeten staan. Als je erop klikt zou je normaal naar het hoofdscherm gebracht worden met de melding 'connecting to HC-05' met daarop volgend 'connected, linefollower Maxim, ready to race'. Rechtsboven is een symbool van een gordel die dicht is, met deze knop kan je ook verbinding maken. Eens je verbonden bent kan je de commando's ingeven.

#### commando's (bij elk commando dat je stuurt zou je een antwoord moeten krijgen van de serial bluetooth terminal.)
debug [on/off]  
start: type 'Start' op u gsm en de auto zal starten met rijden.
stop: type opnieuw 'Start' op u gsm en de auto zal stopppen met rijden.
set cycle [µs]  300
set power [0..255] 70 
set diff [0..1]  0,3 
set kp [0..]  7 
set ki [0..]  0
set kd [0..]  0
calibrate black (dit is voor de kleur zwart te kalibreren.
calibrate white  (dit is voor de kleur wit te kalibreren.

### kalibratie (bij elk commando dat je stuurt zou je een antwoord moeten krijgen van de serial bluetooth terminal.)
uitleg kalibratie
Calibrate black: dit is voor het instellen van de kleur zwart. Hou de sensor van de auto boven een zwart oppervlak en type 'Calibrate black' en verstuur. Je zou een antwoord moeten krijgen.)
Calibrate white: dit is voor het instellen van de kleur wit. Hou de sensor van de auto boven een wit oppervlak en type 'Calibrate white' en verstuur. Je zou een antwoord moeten krijgen.
Set power: dit is voor het kiezen hoe snel vooruit u auto rijdt. Valt de auto stil dan verhoog je deze waarde.
Set diff: dit is voor de snelheid in de bochten. Versnelt de auto in de bochten dan verlaag je deze waarde. Vertraagt de auto in de bochten dan verhoog je deze waarde.
Set kp: Dit is voor hoe snel de robot bijstuurt. Stuurt de robot niet snel genoeg bij dan verhoog je deze waarde. Als de robot over de zwarte lijn waggelt dan verlaag je deze waarde.
Set ki: 
Set kd: 

### settings
De robot rijdt stabiel met volgende parameters:Power 70, Diff 0,3, Kp 8, Ki 0, Kd 0 en cycle 300.

### start/stop button
uitleg locatie + werking start/stop button
Druk op de drukknop en de auto zal beginnen met rijden, als je nogmaals drukt op de drukknop zal de auto stoppen met rijden. De locatie is zodanig gekozen dat je er redelijk makkelijk aankan.
