import Component, {html} from '../Component.js';
import OrbitRepository from '../OrbitRepository.js';
import {fixRoundingError} from '../utils.js';

import './OrbitSelector.js';
import './SaveOrbitButton.js';

class HohmannTransfer extends Component {
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
                <li>Initial Orbit</li>
                <li>Final Orbit</li>
            </ol>
            <div id="errorMessage"></div>
            <orbit-selector max-orbits="2" on:orbitselected="handleOrbitSelected"></orbit-selector>

            <div id="selectedOrbits"></div>

            <dl id="dataContainer">
                <dt>∆V<sub>1</sub> =</dt>
                <dd id="deltaV1">&nbsp;</dd>

                <dt>∆V<sub>2</sub> =</dt>
                <dd id="deltaV2">&nbsp;</dd>

                <dt>∆V<sub>Total</sub> =</dt>
                <dd id="deltaVTotal">&nbsp;</dd>

                <dt>Time Of Flight =</dt>
                <dd id="timeOfFlight">&nbsp;</dd>
            </dl>

            <save-orbit-button id="saveTransferOrbit">Save Transfer Orbit</save-orbit-button>
        `;
    }

    renderSelectedOrbits(selectedOrbits) {
        const orbitFromName = selectedOrbits[0] ? selectedOrbits[0].name : '&lt;Select an Orbit&gt;';
        const orbitToName = selectedOrbits[1] ? selectedOrbits[1].name : '&lt;Select an Orbit&gt;';

        this.selectedOrbitsElement.innerHTML = `Initial: ${orbitFromName}, Final: ${orbitToName}`;
    }

    handleOrbitSelected({detail: selectedOrbits}) {
        this.renderSelectedOrbits(selectedOrbits);

        if (selectedOrbits.length === 2) {
            this.setOrbits(selectedOrbits[0].orbit, selectedOrbits[1].orbit);
        }
    }

    setOrbits(orbitFrom, orbitTo) {
        this.errorMessageElement.innerText = '';
        let transfer;

        try {
            transfer = new Module.HohmannTransfer(orbitFrom, orbitTo);
        } catch (e) {
            console.log(e)
            this.errorMessageElement.innerText = 'Both orbits must be circular to perform a Hohmann transfer.';
            return;
        }

        this.deltaV1Element.innerHTML = fixRoundingError(transfer.deltaV1) + ' km/s';
        this.deltaV2Element.innerHTML = fixRoundingError(transfer.deltaV2) + ' km/s';
        this.deltaVTotalElement.innerHTML = fixRoundingError(transfer.deltaVTotal) + ' km/s'
        this.timeOfFlightElement.innerHTML = fixRoundingError(transfer.timeOfFlight) + 's';

        this.saveTransferOrbitElement.setOrbit(transfer.transferOrbit);

        Module._free(transfer);
    }
}

window.customElements.define('hohmann-transfer', HohmannTransfer);