import Component, {html} from '../Component.js';
import OrbitRepository from '../OrbitRepository.js';
import {fixRoundingError} from '../utils.js';

import './OrbitSelector.js';
import './SaveOrbitButton.js';

class PatchedConicApproximation extends Component {
    static get template() {
        return html`
            <style>
                dt, dd {
                    float: left;
                    width: 50%;
                    margin: 0;
                }

                dt {
                    clear: both;
                }

                #dataContainer {
                    width: 400px;
                }

                #errorMessage {
                    color: red;
                }
            </style>


            <h3>Please select 4 orbits in this order: </h3>
            <ol>
                <li>Parking orbit about origin</li>
                <li>Origin planet about sun</li>
                <li>Parking orbit about target</li>
                <li>Target planet about sun</li>
            </ol>
            <orbit-selector max-orbits="4" on:orbitselected="handleOrbitSelected"></orbit-selector>

            <div id="selectedOrbits"></div>

            <dl id="dataContainer">
                <dt>Transfer Orbit Semimajor Axis =</dt>
                <dd id="aTransfer">&nbsp;</dd>

                <dt>∆V<sub>boost</sub> =</dt>
                <dd id="deltaVBoost">&nbsp;</dd>

                <dt>∆V<sub>retro</sub> =</dt>
                <dd id="deltaVRetro">&nbsp;</dd>

                <dt>∆V<sub>mission</sub> =</dt>
                <dd id="deltaVMission">&nbsp;</dd>

                <dt>V<sub>inf-origin</sub> =</dt>
                <dd id="vInfOrigin">&nbsp;</dd>

                <dt>V<sub>inf-target</sub> =</dt>
                <dd id="vInfTarget">&nbsp;</dd>

                <dt>Time of Flight =</dt>
                <dd id="timeOfFlight">&nbsp;</dd>
            </dl>

        `;
    }

    renderSelectedOrbits(selectedOrbits) {
        const originParkingOrbit = selectedOrbits[0] ? selectedOrbits[0].name : '&lt;Select an Orbit&gt;';
        const originPlanetOrbit = selectedOrbits[1] ? selectedOrbits[1].name : '&lt;Select an Orbit&gt;';
        const targetParkingOrbit = selectedOrbits[2] ? selectedOrbits[2].name : '&lt;Select an Orbit&gt;';
        const targetPlanetOrbit = selectedOrbits[3] ? selectedOrbits[3].name : '&lt;Select an Orbit&gt;';

        this.selectedOrbitsElement.innerHTML = `Origin Park: ${originParkingOrbit}, Origin Planet: ${originPlanetOrbit}, Target Park: ${targetParkingOrbit}, Target Planet: ${targetPlanetOrbit}`;
    }

    handleOrbitSelected({detail: selectedOrbits}) {
        this.renderSelectedOrbits(selectedOrbits);

        if (selectedOrbits.length === 4) {
            this.setOrbits(selectedOrbits[0].orbit, selectedOrbits[1].orbit, selectedOrbits[2].orbit, selectedOrbits[3].orbit);
        }
    }

    setOrbits(originParkingOrbit, originPlanetOrbit, targetParkingOrbit, targetPlanetOrbit) {
        const pca = new Module.PatchedConicApproximation(originParkingOrbit, originPlanetOrbit, targetParkingOrbit, targetPlanetOrbit);

        this.aTransferElement.innerHTML = fixRoundingError(pca.transferOrbit.elements.a) + ' km';
        this.deltaVBoostElement.innerHTML = fixRoundingError(pca.deltaVBoost) + ' km/s';
        this.deltaVRetroElement.innerHTML = fixRoundingError(pca.deltaVRetro) + ' km/s';
        this.deltaVMissionElement.innerHTML = fixRoundingError(pca.deltaVMission) + ' km/s';
        this.vInfOriginElement.innerHTML = fixRoundingError(pca.vInfOrigin) + ' km/s';
        this.vInfTargetElement.innerHTML = fixRoundingError(pca.vInfTarget) + ' km/s';
        this.timeOfFlightElement.innerHTML = fixRoundingError(pca.timeOfFlight) + 's';

        Module._free(pca);
    }
}

window.customElements.define('patched-conic-approximation', PatchedConicApproximation);