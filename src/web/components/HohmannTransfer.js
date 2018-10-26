import Component, {html} from '../Component.js';
import OrbitRepository from '../OrbitRepository.js';
import {fixRoundingError} from '../utils.js';

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


            <div id="errorMessage"></div>
            <div id="orbits"></div>

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
        `;
    }

    constructor() {
        super();

        this.selectedOrbits = [];
        this.orbitRepository = new OrbitRepository();
    }

    connectedCallback() {
        this.renderOrbits();
    }

    renderOrbits() {
        const orbits = this.orbitRepository.getOrbits();

        const orbitList = Object.entries(orbits).map(([name, orbit]) => `<li>${name} <om-button data-name="${name}" on:click="selectOrbit">Select</om-button></li>`)

        this.orbitsElement.innerHTML = `<ul>${orbitList.join('')}</ul>`;
        this.parseTree();
    }

    renderSelectedOrbits() {
        const orbitFromName = this.selectedOrbits[0] ? this.selectedOrbits[0].name : '&lt;Select an Orbit&gt;'
        const orbitToName = this.selectedOrbits[1] ? this.selectedOrbits[1].name : '&lt;Select an Orbit&gt;'

        this.selectedOrbitsElement.innerHTML = `From: ${orbitFromName}, To: ${orbitToName}`;
    }

    selectOrbit({target}) {
        const orbitName = target.dataset.name;

        this.selectedOrbits.push({
            name: orbitName,
            orbit: this.orbitRepository.getOrbitByName(orbitName)
        });

        this.selectedOrbits = this.selectedOrbits.slice(this.selectedOrbits.length - 2, this.selectedOrbits.length);
        this.renderSelectedOrbits();

        if (this.selectedOrbits.length === 2) {
            this.setOrbits(this.selectedOrbits[0].orbit, this.selectedOrbits[1].orbit);
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
    }
}

window.customElements.define('hohmann-transfer', HohmannTransfer);