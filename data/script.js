async function fetchSensorDataLoop() {
  try {
    const response = await fetch('/sensor');
    if (!response.ok) throw new Error('Netzwerkfehler');

    const data = await response.json();

    // Daten anzeigen
    document.getElementById('sat').textContent = data.SAT_COUNT;
    document.getElementById('lat').textContent = data.LAT.toFixed(6);
    document.getElementById('lon').textContent = data.LON.toFixed(6);
    document.getElementById('alt').textContent = data.ALT.toFixed(1);
    document.getElementById('temp').textContent = data.TEMP.toFixed(1);
    document.getElementById('magx').textContent = data.MAGX.toFixed(3);
    document.getElementById('magy').textContent = data.MAGY.toFixed(3);
    document.getElementById('magz').textContent = data.MAGZ.toFixed(3);
    document.getElementById('accx').textContent = data.ACCX.toFixed(3);
    document.getElementById('accy').textContent = data.ACCY.toFixed(3);
    document.getElementById('accz').textContent = data.ACCZ.toFixed(3);
    document.getElementById('gyrox').textContent = data.GYROX.toFixed(3);
    document.getElementById('gyroy').textContent = data.GYROY.toFixed(3);
    document.getElementById('gyroz').textContent = data.GYROZ.toFixed(3);
    document.getElementById('tick').textContent = data.TICK;

    const declination = estimateDeclination(data.LAT, data.LON);
    updateKompassFromMagnetometer(data.MAGX, data.MAGY, data.MAGZ, declination);

  } catch (error) {
    console.error('Fehler beim Laden der Sensor-Daten:');
  } finally {
    setTimeout(fetchSensorDataLoop, 1000);
  }
}

function updateKompassFromMagnetometer(magX, magY, magZ, declination) {
  console.log("Kompass-Update aufgerufen");
  console.log("RAW Magnetwerte:", { magX, magY, magZ });

  if (typeof magX !== 'number' || typeof magY !== 'number' || typeof magZ !== 'number') {
    console.warn("❌ Ungültige Magnetometerdaten");
    return;
  }

  let heading = Math.atan2(magY, magX) * 180 / Math.PI;
  if (isNaN(heading)) {
    console.warn("❌ heading ist NaN");
    return;
  }

  if (heading < 0) heading += 360;
  heading += declination;
  if (heading >= 360) heading -= 360;

  const svgHeading = heading - 90;

  const nadel = document.getElementById("nadel");
  if (nadel) {
    console.log(`→ Drehe SVG-Nadel: rotate(${svgHeading} 0 0)`);
    nadel.setAttribute("transform", `rotate(${svgHeading} 0 0)`);
  } else {
    console.error("❌ #nadel nicht gefunden");
  }

  const gradAnzeige = document.getElementById("kompassGrad");
  if (gradAnzeige) {
    gradAnzeige.textContent = `${Math.round(heading)}° Nord`;
  }
}

function estimateDeclination(lat, lon) {
  if (lat > 40 && lat < 55 && lon > 5 && lon < 15) {
    return 2.5; // Deutschland
  } else if (lat > 35 && lat < 45 && lon > -10 && lon < 0) {
    return 0.5; // Spanien/Portugal
  } else if (lat > 30 && lat < 50 && lon < -90) {
    return -6.0; // USA Midwest
  } else if (lat > 25 && lat < 50 && lon > -80 && lon < -70) {
    return -13.0; // USA East
  } else if (lat > -45 && lat < -10 && lon > 110 && lon < 155) {
    return 11.0; // Australien
  }
  return 0.0; // Default
}

fetchSensorDataLoop();
