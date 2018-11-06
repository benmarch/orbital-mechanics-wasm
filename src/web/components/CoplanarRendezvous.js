import Component, {html} from '../Component.js';
import OrbitRepository from '../OrbitRepository.js';
import {fixRoundingError} from '../utils.js';

import './OrbitSelector.js';
import './SaveOrbitButton.js';

class CoplanarRendezvous extends Component {
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


            <h3>Please select 2 orbits in this order: </h3>
            <ol>
                <li>Interceptor Orbit</li>
                <li>Target Orbit</li>
            </ol>
            <div id="errorMessage"></div>
            <orbit-selector max-orbits="2" on:orbitselected="handleOrbitSelected"></orbit-selector>

            <div id="selectedOrbits"></div>

            <dl id="dataContainer">
                <dt>Time of Flight =</dt>
                <dd id="timeOfFlight">&nbsp;</dd>

                <dt>ω<sub>int</sub> =</dt>
                <dd id="intAngularV">&nbsp;</dd>

                <dt>ω<sub>tgt</sub> =</dt>
                <dd id="tgtAngularV">&nbsp;</dd>

                <dt>α<sub>lead</sub> =</dt>
                <dd id="leadAngle">&nbsp;</dd>

                <dt>φ<sub>f</sub> =</dt>
                <dd id="phaseAngle">&nbsp;</dd>

                <dt>Wait Time =</dt>
                <dd id="waitTime">&nbsp;</dd>
            </dl>

        `;
    }

    renderSelectedOrbits(selectedOrbits) {
        const orbitFromName = selectedOrbits[0] ? selectedOrbits[0].name : '&lt;Select an Orbit&gt;';
        const orbitToName = selectedOrbits[1] ? selectedOrbits[1].name : '&lt;Select an Orbit&gt;';

        this.selectedOrbitsElement.innerHTML = `Interceptor: ${orbitFromName}, Target: ${orbitToName}`;
    }

    handleOrbitSelected({detail: selectedOrbits}) {
        this.renderSelectedOrbits(selectedOrbits);

        if (selectedOrbits.length === 2) {
            this.setOrbits(selectedOrbits[0].orbit, selectedOrbits[1].orbit);
        }
    }

    setOrbits(orbitFrom, orbitTo) {
        this.errorMessageElement.innerText = '';
        let rendezvous;

        try {
            rendezvous = new Module.CoplanarRendezvous(orbitFrom, orbitTo);
        } catch (e) {
            console.log(e);
            this.errorMessageElement.innerText = 'Orbits must be coplanar!';
            return;
        }

        this.timeOfFlightElement.innerHTML = fixRoundingError(rendezvous.timeOfFlight) + 's';
        this.intAngularVElement.innerHTML = fixRoundingError(rendezvous.interceptorAngularVelocity) + ' rad/s';
        this.tgtAngularVElement.innerHTML = fixRoundingError(rendezvous.targetAngularVelocity) + ' rad/s';
        this.leadAngleElement.innerHTML = fixRoundingError(rendezvous.leadAngle) + ' rad';
        this.phaseAngleElement.innerHTML = fixRoundingError(rendezvous.phaseAngle) + ' rad';
        this.waitTimeElement.innerHTML = fixRoundingError(rendezvous.waitTime) + ' s';

        Module._free(rendezvous);
    }
}

window.customElements.define('coplanar-rendezvous', CoplanarRendezvous);