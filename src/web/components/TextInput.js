import Component, {html} from '../Component.js';

class TextInput extends Component {
    static get template() {
        return html`
            <style>
                :host {
                    display: inline-block;
                }

                #input {
                    box-sizing: border-box;
                    width: 100%;
                    -webkit-appearance: none;
                    display: inline-block;
                    padding: 5px 10px;
                    background-color: #fff;
                    border: var(--input-border);

                    color: var(--text-color);
                    font-family: var(--font-family);
                    font-size: var(--font-size);
                    font-weight: var(--font-weight);
                    letter-spacing: var(--letter-spacing);
                    line-height: var(--line-height);
                }
            </style>

            <input id="input" on:input="handleChange" on:change="handleChange" @:type @:value @:min @:max />
        `;
    }

    static get observedAttributes() {
        return ['value'];
    }

    set value(value) {
        if (this.inputElement.value !== value) {
            this.inputElement.value = value;
        }
    }
    get value() { return this.inputElement.value }

    constructor() {
        super();
        this.name = this.getAttribute('name');
    }

    attributeChangedCallback(attrName, oldVal, newVal) {
        if (newVal !== this.value) {
            this.value = newVal;
        }
    }

    handleChange(event) {
        this.setAttribute('value', event.target.value);
        this.dispatchEvent(new Event(event.type));
    }
}

window.customElements.define('text-input', TextInput);