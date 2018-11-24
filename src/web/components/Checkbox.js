import Component, {html} from '../Component.js';

class Checkbox extends Component {
    static get template() {
        return html`
            <style>
                #checkbox {
                    -webkit-appearance: none;
                    display: inline-block;
                    padding: 10px;
                    background-color: #fff;
                    border: var(--input-border);
                    cursor: pointer;
                    position: relative;

                    color: var(--text-color);
                    font-family: var(--font-family);
                    font-size: var(--font-size);
                    font-weight: var(--font-weight);
                    letter-spacing: var(--letter-spacing);
                    line-height: var(--line-height);
                }

                #checkbox:checked::after {
                    content: "✔";
                    position: absolute;
                    top: 3px;
                    left: 3px;
                    line-height: 1;
                }
            </style>

            <input type="checkbox" id="checkbox" on:change="handleChange">
        `;
    }

    get checked() {
        return this.checkboxElement.checked;
    }

    handleChange() {
        this.dispatchEvent(new Event('change', {detail: {checked: this.checked}}));
    }
}

window.customElements.define('om-checkbox', Checkbox);