import Component, {html} from '../Component.js';
import OrbitRepository from '../OrbitRepository.js';
import {fixRoundingError} from '../utils.js';

import './OrbitSelector.js';
import './AdjustableInput.js';

class J2EffectCalculator extends Component {
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


            <h3>Please select an orbit: </h3>
            <div id="errorMessage"></div>
            <orbit-selector max-orbits="1" on:orbitselected="handleOrbitSelected"></orbit-selector>

            <div id="selectedOrbits"></div>

            <dl id="dataContainer">
                <dt>Rate of Argument of Perigee change (∆ω/∆t) = </dt>
                <dd id="aopChange"></dd>

                <dt>Rate of Right Ascension change (∆Ω/∆t) = </dt>
                <dd id="raChange"></dd>

                <dt>Rate of Mean Anomaly change (∆M/∆t) = </dt>
                <dd id="mChange"></dd>
            </dl>
        `;
    }

    renderSelectedOrbits(selectedOrbits) {
        const orbitName = selectedOrbits[0] ? selectedOrbits[0].name : '&lt;Select an Orbit&gt;';

        this.selectedOrbitsElement.innerHTML = `Orbit: ${orbitName}`;
    }

    handleOrbitSelected({detail: selectedOrbits}) {
        this.renderSelectedOrbits(selectedOrbits);

        if (selectedOrbits.length === 1) {
            this.setOrbit(selectedOrbits[0].orbit);
        }
    }

    setOrbit(orbit) {
        this.j2Effects = new Module.J2EffectCalculator(orbit);
        this.render();
    }

    render() {
        this.aopChangeElement.innerHTML = fixRoundingError(this.j2Effects.argumentOfPerigeeRateOfChange) + ' rad/s';
        this.raChangeElement.innerHTML = fixRoundingError(this.j2Effects.rightAscensionRateOfChange) + ' rad/s';
        this.mChangeElement.innerHTML = fixRoundingError(this.j2Effects.meanAnomalyRateOfChange) + ' rad/s';
    }
}

window.customElements.define('j2-effect-calculator', J2EffectCalculator);