import Component, { html } from '../Component.js';
import './TextInput.js'

export default class AdjustableInput extends Component {
    static get template() {
        return html`
            <style>
                :host {
                    display: block;
                    contain: content;
                }
            </style>

            <div data-component="AdjustableInput" class="adjustable-input">
                <input type="range" id="range" on:input="handleInput">
                <text-input type="number" id="number" on:input="handleInput"></text-input>
            </div>
        `;
    }

    static get observedAttributes() {
        return ['value'];
    }

    constructor() {
        super();

        this.initialized = false;
        this.min = 0;
        this.max = 100;
        this.step = 1;
        this.value = 0;

        this.value = this.getAttribute('value') || this.value;
        this.name = this.getAttribute('name') || '';
    }

    connectedCallback() {
        // get properties
        this.max = this.rangeElement.max = this.getAttribute('max') || this.max;
        this.min = this.rangeElement.min = this.getAttribute('min') || this.min;
        this.step = this.rangeElement.step = this.getAttribute('step') || this.step;

        this.initialized = true;
        this.updateValue(this.value);
    }

    attributeChangedCallback(attrName, oldVal, newVal) {
        if (!this.initialized) return;

        if (attrName === 'value') {
            this.updateValue(Number(newVal));
        }
    }

    handleInput(event) {
        this.updateValue(event.target.value, event.target);
        this.setAttribute('value', event.target.value);
        this.dispatchEvent(new Event('input'));
    }

    updateValue(value) {
        if (Number(this.rangeElement.value) !== value) {
            this.rangeElement.value = value;
        }

        if (Number(this.numberElement.value) !== value) {
            this.numberElement.value = value;
        }

        this.value = value;
    }
}

window.customElements.define('adjustable-input', AdjustableInput);
