import Component, {html} from '../Component.js';
import {fixRoundingError} from '../utils.js';

import './OrbitSelector.js';
import './TextInput.js';

class RocketStaging extends Component {
    static get template() {
        return html`
            <style>
                :host {
                    display: grid;
                    column-gap: var(--gutter-width);
                    grid-template-columns: 1fr 1fr;
                    grid-template-areas: "left right";
                }

                #left {
                    grid-area: left;
                }

                #right {
                    grid-area: right;
                }

                dt, dd {
                    float: left;
                    width: 50%;
                    margin: 0;
                }

                dt {
                    clear: both;
                }

                .data-container {
                    float: left;
                    width: 700px;
                }
            </style>

            <section id="left">
                <h4>Rocket Data:</h4>
                <dl class="data-container">
                    <dt>Payload Mass (kg):</dt>
                    <dd><text-input type="number" id="payloadMass"></text-input></dd>

                    <dt>Total Mass (kg) = </dt>
                    <dd id="totalMass">0 kg</dd>

                    <dt>Total ∆V = </dt>
                    <dd id="totalDeltaV">0 km/s</dd>
                </dl>

                <h5>Fill out all the fields (including stages) and then calculate:</h5>
                <om-button on:click="calculate">Calculate</om-button>
            </section>

            <section id="right">
                <h4>Select Number of Stages</h4>
                <select id="numStages" on:change="generateStages">
                    <option value="3">Select One</option>
                    <option value="1">1</option>
                    <option value="2">2</option>
                    <option value="3">3</option>
                    <option value="4">4</option>
                    <option value="5">5</option>
                </select>

                <div id="stageData"></div>
            </section>
        `;
    }

    constructor() {
        super();

        this.rocketStaging = new Module.RocketStaging();
        this.rocket = {
            payloadMass: 0,
            totalMass: 0,
            deltaV: 0,
            stages: [],
            stage1: this.createEmptyStage(),
            stage2: this.createEmptyStage(),
            stage3: this.createEmptyStage(),
            stage4: this.createEmptyStage(),
            stage5: this.createEmptyStage(),
        };
    }

    createEmptyStage() {
        return {
            structureMass: 0,
            propellantMass: 0,
            specificImpulse: 0,
            initialMass: 0,
            finalMass: 0,
            deltaV: 0,
        };
    }

    generateStages() {
        const numStages = Number(this.numStagesElement.value || 0);

        let stageMarkup = '';

        for (let i = 0; i < numStages; i++) {
            this.rocket.stages.push(this.createEmptyStage());

            stageMarkup += `
                <h4>Stage ${i + 1}:</h4>
                <dl class="data-container">
                    <dt>Structure Mass (kg):</dt>
                    <dd><text-input type="number" name="structureMass" on:change="updateField" data-stage-id="${i}" ></text-input></dd>

                    <dt>Propellant Mass (kg):</dt>
                    <dd><text-input type="number" name="propellantMass" on:change="updateField" data-stage-id="${i}" ></text-input></dd>

                    <dt>Specific Impulse (s<sup>-1</sup>):</dt>
                    <dd><text-input type="number" name="specificImpulse" on:change="updateField" data-stage-id="${i}" ></text-input></dd>

                    <dt>Initial Mass (kg) = </dt>
                    <dd id="stage${i}InitialMass">0 kg</dd>

                    <dt>Final Mass (kg) = </dt>
                    <dd id="stage${i}FinalMass">0 kg</dd>

                    <dt>Stage ∆V (km/s) = </dt>
                    <dd id="stage${i}DeltaV">0 km/s</dd>
                </dl>
            `
        }

        this.stageDataElement.innerHTML = stageMarkup;
        this.parseTree();
    }

    calculate() {
        this.rocket.payloadMass = Number(this.payloadMassElement.value || 0);

        this.rocket.stages.forEach((stage, i) => {
            this.rocket[`stage${i+1}`] = stage;
        });

        this.rocket = this.rocketStaging.calculateTotalDeltaV(this.rocket);

        this.rocket.stages = [];
        [this.rocket.stage1, this.rocket.stage2, this.rocket.stage3, this.rocket.stage4, this.rocket.stage5].forEach((stage, i) => {
            if (stage.specificImpulse) {
                this.rocket.stages.push(stage);
            }
        });

        this.render();
    }

    updateField({target}) {
        this.rocket.stages[target.dataset.stageId][target.name] = Number(target.value || 0);
    }

    render() {
        this.totalMassElement.innerHTML = fixRoundingError(this.rocket.totalMass) + ' kg';
        this.totalDeltaVElement.innerHTML = fixRoundingError(this.rocket.deltaV) + ' km/s';

        this.rocket.stages.forEach((stage, i) => {
            this[`stage${i}InitialMassElement`].innerHTML = fixRoundingError(stage.initialMass) + ' kg';
            this[`stage${i}FinalMassElement`].innerHTML = fixRoundingError(stage.finalMass) + ' kg';
            this[`stage${i}DeltaVElement`].innerHTML = fixRoundingError(stage.deltaV) + ' km/s';
        });
    }
}

window.customElements.define('rocket-staging', RocketStaging);