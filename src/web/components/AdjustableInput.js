import Component, { html } from '../Component.js';

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
                <input type="range" class="adjustable-input__range">
                <input type="number" class="adjustable-input__number">
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

        // save references
        this.rangeInput = this.shadowRoot.querySelector('.adjustable-input__range');
        this.numberInput = this.shadowRoot.querySelector('.adjustable-input__number')
    }

    connectedCallback() {
        // get properties
        this.max = this.rangeInput.max = this.getAttribute('max') || this.max;
        this.min = this.rangeInput.min = this.getAttribute('min') || this.min;
        this.step = this.rangeInput.step = this.getAttribute('step') || this.step;
        this.value = this.getAttribute('value') || this.value;
        this.name = this.getAttribute('name') || '';

        this.registerListeners();

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
        this.updateValue(event.target.value);
        this.setAttribute('value', event.target.value);
        this.dispatchEvent(new Event('input'));
    }

    registerListeners() {
        this.rangeInput.addEventListener('input', this.handleInput.bind(this));
        this.numberInput.addEventListener('input', this.handleInput.bind(this));
    }

    updateValue(value) {
        this.rangeInput.value = value;
        this.numberInput.value = value;
        this.value = value;
    }
}

window.customElements.define('adjustable-input', AdjustableInput);
