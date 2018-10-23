import Component, { html } from '../Component.js';
import OrbitRepository from '../OrbitRepository.js';
import './StateVectorGenerator.js';
import './OrbitalElementsGenerator.js';
import './Button.js';

export default class CreateOrbit extends Component {
    static get template() {
        return html`
            <style>
                :host {
                    display: block;
                    padding: var(--gutter-width);
                }

                #container {
                    display: grid;
                    column-gap: var(--gutter-width);
                    grid-template-columns: 2fr 1fr 2fr;
                    grid-template-areas: "left center right";
                }

                #stateVectorGenerator {
                    grid-area: left;
                }

                #orbitalElementsGenerator {
                    grid-area: right;
                }

                #copyActions {
                    grid-area: center;
                    align-self: center;
                }
            </style>

            <div id="container">
                <state-vector-generator id="stateVectorGenerator" on:input="handleStateVectorInput"></state-vector-generator>
                <orbital-elements-generator id="orbitalElementsGenerator" on:input="handleOrbitalElementsInput"></orbital-elements-generator>

                <div id="copyActions">
                    <om-button part="button" id="transferStateVectorsButton" on:click="transferStateVectors">
                        Copy State Vectors &rightarrow;
                    </om-button>
                    <label>
                        <span>Auto</span>
                        <input type="checkbox" on:change="toggleAutoTransferSV">
                    </label>
                    <br><br>
                    <om-button part="button" id="transferOrbitalElementsButton" on:click="transferOrbitalElements">
                        &leftarrow; Copy Orbital Elements
                    </om-button>
                    <label>
                        <span>Auto</span>
                        <input type="checkbox" on:change="toggleAutoTransferOE">
                    </label>
                </div>
            </div>
        `;
    }

    constructor() {
        super();

        this.orbitRepository = new OrbitRepository();
        this.shouldAutoTransferSV = false;
        this.shouldAutoTransferOE = false;
    }

    connectedCallback() {
        if (this.routeData && this.routeData.name) {
            const orbitToLoad = this.orbitRepository.getOrbitByName(this.routeData.name);

            if (orbitToLoad) {
                this.stateVectorGeneratorElement.updateWithOrbitalElements(orbitToLoad.elements);
                this.orbitalElementsGeneratorElement.updateWithStateVectors(orbitToLoad.stateVectors);
            }
        }
    }

    handleStateVectorInput() {
        if (this.shouldAutoTransferSV) {
            this.transferStateVectors();
        }
    }

    handleOrbitalElementsInput() {
        if (this.shouldAutoTransferOE) {
            this.transferOrbitalElements();
        }
    }

    transferStateVectors() {
        const stateVectors = this.stateVectorGeneratorElement.getStateVectors();
        this.orbitalElementsGeneratorElement.updateWithStateVectors(stateVectors);
    }

    transferOrbitalElements() {
        const elements = this.orbitalElementsGeneratorElement.getElements();
        this.stateVectorGeneratorElement.updateWithOrbitalElements(elements);
    }

    toggleAutoTransferSV(event) {
        this.shouldAutoTransferSV = event.target.checked;
    }

    toggleAutoTransferOE(event) {
        this.shouldAutoTransferOE = event.target.checked;
    }
}

window.customElements.define('create-orbit', CreateOrbit);