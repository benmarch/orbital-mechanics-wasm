import Component, {html} from '../Component.js';
import OrbitRepository from '../OrbitRepository.js';
import {fixRoundingError} from '../utils.js';

import './OrbitSelector.js';
import './AdjustableInput.js';

class PlaneChange extends Component {
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


            <div id="errorMessage"></div>
            <orbit-selector max-orbits="2" on:orbitselected="handleOrbitSelected"></orbit-selector>

            <div id="selectedOrbits"></div>

            <dl id="dataContainer">
                <dt>∆Theta:</dt>
                <dd><adjustable-input id="deltaTheta" type="range" min="-360" max="360" value="0" on:input="handleThetaChange"></adjustable-input></dd>

                <dt>∆V<sub>Total</sub> =</dt>
                <dd id="deltaVTotal">&nbsp;</dd>
            </dl>
        `;
    }

    renderSelectedOrbits(selectedOrbits) {
        const orbitFromName = selectedOrbits[0] ? selectedOrbits[0].name : '&lt;Select an Orbit&gt;'
        const orbitToName = selectedOrbits[1] ? selectedOrbits[1].name : '&lt;Select an Orbit&gt;'

        this.selectedOrbitsElement.innerHTML = `From: ${orbitFromName}, To: ${orbitToName}`;
    }

    handleOrbitSelected({detail: selectedOrbits}) {
        this.renderSelectedOrbits(selectedOrbits);

        if (selectedOrbits.length === 2) {
            this.setOrbits(selectedOrbits[0].orbit, selectedOrbits[1].orbit)
        }
    }

    handleThetaChange() {
        if (this.planeChange) {
            this.planeChange.setPlaneChangeAngle(Number(this.deltaThetaElement.value) * Math.PI / 180);
            this.render();
        }
    }

    setOrbits(orbitFrom, orbitTo) {
        this.planeChange = new Module.PlaneChange(orbitFrom, orbitTo);
        this.handleThetaChange();
    }

    render() {
        this.deltaVTotalElement.innerHTML = fixRoundingError(this.planeChange.deltaV) + ' km/s';
    }
}

window.customElements.define('plane-change', PlaneChange);